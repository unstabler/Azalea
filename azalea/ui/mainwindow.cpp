#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>

#include <QQmlContext>
#include <QQuickItem>
#include <ui/timeline/statusadapter.hpp>
#include <ui/timeline/notificationstatusadapter.hpp>

#include "mastodon/apicontext.hpp"
#include "mastodon/mastodonapi.hpp"

#include "azaleaapplication.hpp"
#include "singleton.hpp"
#include "configmanager.hpp"
#include "settingswindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _timelineModel(),
    _configManager(Singleton<ConfigManager>::getInstance()),
    _apiContext(new APIContext(this))
{
    ui->setupUi(this);
    ui->retranslateUi(this);
    
    connect(this, &MainWindow::quit, this, &MainWindow::close, Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    // connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::updateTimeline);
    connect(ui->actionConfig, &QAction::triggered, this, [this] {
         SettingsWindow window(this);
         window.exec();
    });
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);

    _configManager.load();
    if (_configManager.credentials()->length() <= 0) {
        if(QMessageBox::question(
                this,
                this->windowTitle(),
                tr("NO_TOKEN_SET")
        ) == QMessageBox::Yes) {
            this->addAccount();
        } else {
            this->quit();
        }
    }
    
    
    this->initializeShortcuts();
    ui->timelineView->setSource(QUrl(QStringLiteral("qrc:/components/Timeline.qml")));
    
    if (_configManager.credentials()->empty()) {
        return;
    }
    
    auto defaultCredential = _configManager.credentials()->first();
    QTimer::singleShot(0, [=] {
        this->initializeWith(defaultCredential);
    });
}

void MainWindow::initializeShortcuts()
{
    int i = 0;
    for (auto timelineType : TimelineType::getAsIterable()) {
        auto *shortcut = new QShortcut(Qt::Key_1 + (i++), this);
        shortcut->setContext(Qt::ApplicationShortcut);
        shortcut->setAutoRepeat(false);
        shortcut->setEnabled(true);
        _timelineShortcuts[timelineType] = shortcut;
        connect(shortcut, &QShortcut::activated, this, [=] {
            qDebug() << "shortcut: setting timeline to " << timelineType;
            this->setTimeline(timelineType);
        });
    }
}


void MainWindow::initializeWith(Credential *credential)
{
    auto *tlViewContext = ui->timelineView->rootContext();
    tlViewContext->setContextProperty("timelineModel", nullptr);
    
    _apiContext->setHost(credential->instanceName());
    _apiContext->setToken(credential->token()->accessToken);
    _api = std::shared_ptr<MastodonAPI>(new MastodonAPI(_apiContext));
    
    _streamingClient = std::unique_ptr<StreamingClient>(
            new StreamingClient(*_apiContext, "user")
    );
    
    connect(_streamingClient.get(), &StreamingClient::streamEvent, this, &MainWindow::streamEvent);
    
    for (auto timelineType : TimelineType::getAsIterable()) {
        _timelineModel[timelineType] = std::make_unique<TimelineModel>(
            new TimelineModel(this)
        );
    
        this->updateTimeline(timelineType);
    }
    
    this->setTimeline(TimelineType::HOME);
    _streamingClient->open();
}

void MainWindow::setTimeline(TimelineType::Enum timelineType)
{
    auto *tlViewContext = ui->timelineView->rootContext();
    tlViewContext->setContextProperty("timelineModel", _timelineModel[timelineType].get());
    _currentTimeline = timelineType;
}

void MainWindow::addAccount()
{
    auto instanceName = QInputDialog::getText(
                this,
                tr("INSTANCE_NAME"),
                tr("INPUT_INSTANCE_NAME")
    );

    if (instanceName.isEmpty()) {
        this->quit();
    }

    auto context = new APIContext(this);
    context->setHost(instanceName);
    auto api = new MastodonAPI(context);
    auto response = api->apps()->post(
                AzaleaApplication::APPLICATION_NAME,
                v1::AppsAPI::NO_REDIRECT_URIS,
                "read write follow",
                AzaleaApplication::APPLICATION_WEBSITE
    );
    connect(response, &APIFutureResponse::resolved, [=]() {
        auto application = response->tryDeserialize();
        auto authorizeUrl = api->oauth()->getAuthorizeUrl(
            application->clientId,
            "read write follow"
        );
        qDebug() << QString("created _application #%1").arg(
            application->id
        );
        QDesktopServices::openUrl(authorizeUrl);

        auto authorizationCode = QInputDialog::getText(
                    this,
                    tr("TITLE_AUTHORIZATION_CODE"),
                    tr("TEXT_AUTHORIZATION_CODE")
                    );

        auto response = api->oauth()->tokenByAuthorizationCode(
            application->clientId,
            application->clientSecret,
            authorizationCode);
        connect(response, &APIFutureResponse::resolved, [=]() {
            auto token = response->tryDeserialize();
            auto credential = new Credential(instanceName, "unknown", token.get());
            _configManager.credentials()->push_back(credential);
            _configManager.save();
            qDebug() << token->accessToken;
        });
        connect(response, &APIFutureResponse::rejected, [=](int code, QString content) {
            qDebug() << "ERROR:" << content << QString("%1").arg(code);
        });
    });
    connect(response, &APIFutureResponse::rejected, [=](int code, QString content) {
        qDebug() << "ERROR:" << content << QString("%1").arg(code);
    });
}

void MainWindow::updateTimeline(TimelineType::Enum timelineType, bool clear)
{
    v1::in::TimelinesAPIArgs args;
    
    if (timelineType == TimelineType::MENTIONS) {
        this->updateNotifications(clear); // FIXME
        return;
    }
    
    if (clear) {
        _timelineModel[timelineType]->clear();
    } else if (_timelineModel[timelineType]->count() > 0) {
        auto id = _timelineModel[timelineType]->first()->id();
        args.minId.set(id);
    }
    
    APIFutureResource<ResourceList<v1::Status>> *response = nullptr;
    
    switch (timelineType) {
        case TimelineType::HOME:
            response = _api->timelines()->home(args);
            break;
        case TimelineType::INSTANCE_LOCAL:
            response = _api->timelines()->local(args);
            break;
        case TimelineType::INSTANCE_FEDERATED:
            response = _api->timelines()->federated(args);
            break;
            
        default:
            qWarning() << "updateTimeline(): timelineType " << timelineType << " is not supported yet";
            return;
    }
    
    connect(response, &APIFutureResponse::resolved, this, [=] {
        this->timelineResolved(timelineType, response->tryDeserialize());
    });
    connect(response, &APIFutureResponse::rejected, this, [=] (int code, QString content) {
        // TODO: 오류 핸들링
    });
}

void MainWindow::updateNotifications(bool clear)
{
    v1::in::NotificationListArgs args;
    
    if (clear) {
        _timelineModel[TimelineType::MENTIONS]->clear();
    } else if (_timelineModel[TimelineType::MENTIONS]->count() > 0) {
        auto id = _timelineModel[TimelineType::MENTIONS]->first()->id();
        args.minId.set(id);
    }
    
    auto response = _api->notifications()->list(args);
    connect(response, &APIFutureResponse::resolved, this, [=] {
        this->notificationsResolved(response->tryDeserialize());
    });
}

void MainWindow::timelineResolved(TimelineType::Enum timelineType, QSharedPointer<ResourceList<v1::Status>> statuses)
{
    auto *model = _timelineModel.at(timelineType).get();
    for (auto i = statuses->rbegin(); i != statuses->rend(); i++) {
        model->prepend(new StatusAdapter(this, *i));
    }
}

void MainWindow::notificationsResolved(QSharedPointer<ResourceList<v1::Notification>> notifications)
{
    auto *model = _timelineModel.at(TimelineType::MENTIONS).get();
    for (auto i = notifications->rbegin(); i != notifications->rend(); i++) {
        model->prepend(new NotificationStatusAdapter(this, *i));
    }
}

/**
 * 스트림 이벤트 핸들러
 * @param eventType
 * @param payload
 */
void MainWindow::streamEvent(QString eventType, QJsonObject payload)
{
    if (eventType == "update") {
        QSharedPointer<v1::Status> status(new v1::Status);
        fromJSON<v1::Status>(status.get(), payload);
        _timelineModel.at(TimelineType::HOME)->prepend(new StatusAdapter(this, status));
    }
}

std::shared_ptr<MastodonAPI> MainWindow::api() const
{
    return this->_api;
}

/**
 * keyboard event handler
 * TODO: 혼동 방지를 위해 qml keyboard navigation 끄고 여기로 옮기는게 맞는 것 같다
 * TODO: PgUp / PgDn은 https://doc.qt.io/qt-5/qml-qtquick-listview.html의 indexAt를 써서 계산하는게 맞는 것 같아.
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_U:
            ui->postArea->focusPostArea();
            break;
        case Qt::Key_Escape:
            ui->postArea->blurPostArea();
            break;
        case Qt::Key_F: {
            auto *statusAdapter = getStatusAdapterAtCurrentIndex();
            toggleFavourite(statusAdapter);
        }
            break;
        case Qt::Key_T: {
            auto *statusAdapter = getStatusAdapterAtCurrentIndex();
            toggleBoost(statusAdapter);
        }
            break;
        case Qt::Key_Down:
        case Qt::Key_J:
            setCurrentIndex(getCurrentIndex() + 1);
            break;
        case Qt::Key_Up:
        case Qt::Key_K:
            setCurrentIndex(getCurrentIndex() - 1);
            break;
        case Qt::Key_Space: {
            if (!event->isAutoRepeat()) {
                _refreshKeyPressedAt = QDateTime::currentMSecsSinceEpoch();
            }
            break;
        }
        default:
            goto default_handler;
    }
    
    return;
    
    default_handler:
    QWidget::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    static const int DURATION_MS_FORCE_REFRESH = 500;
    
    if (!event->isAutoRepeat()) {
        switch (event->key()) {
            case Qt::Key_Space: {
                // BUG: postArea에 포커싱 있어도 새로고침 발생함
                if (ui->postArea->isFocused()) {
                    return;
                }
                // force refresh 여부. 500ms 이상 누르고 있으면 타임라인 내용을 전부 지움
                bool clear = QDateTime::currentMSecsSinceEpoch() - _refreshKeyPressedAt > DURATION_MS_FORCE_REFRESH;
                this->updateTimeline(_currentTimeline, clear);
                break;
            }
            default:
                goto default_handler;
        }
    } else {
        goto default_handler;
    }
    
    return;
    
    default_handler:
    QWidget::keyReleaseEvent(event);
}


QQuickItem *MainWindow::getQMLTimeline()
{
    return ui->timelineView->rootObject()->findChild<QQuickItem *>("timeline");
}

void MainWindow::setCurrentIndex(int index)
{
    auto qmlTimeline = getQMLTimeline();
    qmlTimeline->setProperty(
            "currentIndex",
            std::min(
                    std::max(0, index),
                    (qmlTimeline->property("count").toInt() - 1)
            )
    );
}

int MainWindow::getCurrentIndex()
{
    return getQMLTimeline()->property("currentIndex").toInt();
}

StatusAdapterBase *MainWindow::getStatusAdapterAtCurrentIndex()
{
    return _timelineModel.at(_currentTimeline)->at(getCurrentIndex());
}

void MainWindow::toggleBoost(StatusAdapterBase *statusAdapter)
{
    qDebug() << "TODO: implement boost";
    statusAdapter->setBoosted(!statusAdapter->isBoosted());
}

void MainWindow::toggleFavourite(StatusAdapterBase *statusAdapter)
{
    qDebug() << "TODO: implement favourite";
    statusAdapter->setFavourited(!statusAdapter->isFavourited());
}

MainWindow::~MainWindow()
{
    delete ui;
}


