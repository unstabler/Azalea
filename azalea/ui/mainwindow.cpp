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
    _apiContext(Singleton<APIContext>::getInstance()),
    _tootContextMenu(this)
{
    ui->setupUi(this);
    ui->retranslateUi(this);
    
    // FIXME: 설정에 저장할 수 있어야 함
    ui->splitter->setSizes(QList<int>({128, 300}));
    
    connect(this, &MainWindow::quit, this, &MainWindow::close, Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    // connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::updateTimeline);
    connect(ui->actionConfig, &QAction::triggered, this, [this] {
         SettingsWindow window(this);
         window.exec();
    });
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);
    
    connect(this, &MainWindow::initialize, this, [this]{
        _configManager.load();
        if (_configManager.credentials()->length() <= 0) {
            if(QMessageBox::question(
                    this,
                    this->windowTitle(),
                    tr("NO_TOKEN_SET")
            ) == QMessageBox::Yes) {
                this->addAccount();
            } else {
                emit quit();
            }
        }
        
        if (_configManager.credentials()->empty()) {
            return;
        }
        auto defaultCredential = _configManager.credentials()->first();
        
        this->initializeTimelineTabs();
        this->initializeWith(defaultCredential);
    }, Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    
    connect(this, &MainWindow::initialize, this, &MainWindow::initializeShortcuts);
    connect(this, &MainWindow::initialized, ui->postArea, &PostArea::reloadInstanceInfo);
    
    emit initialize();
}

void MainWindow::initializeTimelineTabs()
{
    for (auto timelineType : TimelineType::getAsIterable()) {
        auto* frame = new QFrame(this);
        auto* timelineView = new QQuickWidget(this);
        frame->setLayout(new QVBoxLayout(this));
        frame->layout()->addWidget(timelineView);
        frame->layout()->setContentsMargins(0, 0, 0, 0);
        frame->setFrameShape(QFrame::NoFrame);
        
        timelineView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        timelineView->setResizeMode(QQuickWidget::SizeRootObjectToView);
        timelineView->setSource(QUrl(QStringLiteral("qrc:/components/Timeline.qml")));
        
        _timelineTabs[timelineType] = timelineView;
        ui->timelineTabWidget->addTab(frame, TimelineType::toString(timelineType));
    }
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
            this->setTimeline(timelineType);
        });
    }
}


void MainWindow::initializeWith(Credential *credential)
{
    _apiContext.setHost(credential->instanceName());
    _apiContext.setToken(credential->token()->accessToken);
    _api.reset(new MastodonAPI(&_apiContext));
    
    _streamingClient.reset(
            new StreamingClient(_apiContext, "user")
    );
    
    connect(_streamingClient.get(), &StreamingClient::streamEvent, this, &MainWindow::streamEvent);
    
    for (auto timelineType : TimelineType::getAsIterable()) {
        _timelineModel[timelineType] = std::make_unique<TimelineModel>(
            new TimelineModel(this)
        );
        
        auto *tlViewContext = _timelineTabs[timelineType]->rootContext();
        auto *tlViewRoot = _timelineTabs[timelineType]->rootObject();
        
        tlViewContext->setContextProperty("timelineModel", _timelineModel[timelineType].get());
        connect(tlViewRoot, SIGNAL(rightClicked(QVariant)), this, SLOT(onQMLTimelineRightClicked(QVariant)));
    
        this->updateTimeline(timelineType);
    }
    
    this->setTimeline(TimelineType::HOME);
    _streamingClient->open();
    
    emit initialized();
}

void MainWindow::setTimeline(TimelineType::Enum timelineType)
{
    _currentTimeline = timelineType;
    ui->timelineTabWidget->setCurrentIndex(((int) timelineType) - 1);
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
        case Qt::Key_R: {
            auto *statusAdapter = getStatusAdapterAtCurrentIndex();
            ui->postArea->setReplyTo(statusAdapter);
            ui->postArea->focusPostArea();
        }
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
        case Qt::Key_V:
            showContextMenu();
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
    return _timelineTabs[_currentTimeline]->rootObject()->findChild<QQuickItem *>("timeline");
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
    auto *response = (!statusAdapter->isBoosted()) ?
            _api->statuses()->reblog(statusAdapter->id()) :
            _api->statuses()->unreblog(statusAdapter->id());
    
    connect(response, &APIFutureResponse::resolved, [statusAdapter] {
        statusAdapter->setBoosted(!statusAdapter->isBoosted());
    });
}

void MainWindow::toggleFavourite(StatusAdapterBase *statusAdapter)
{
    auto *response = (!statusAdapter->isFavourited()) ?
            _api->statuses()->favourite(statusAdapter->id()) :
            _api->statuses()->unfavourite(statusAdapter->id());
    
    connect(response, &APIFutureResponse::resolved, [statusAdapter] {
        statusAdapter->setFavourited(!statusAdapter->isFavourited());
    });
}

void MainWindow::onQMLTimelineRightClicked(const QVariant &qVarStatus)
{
    auto *statusAdapter = qvariant_cast<StatusAdapterBase *>(qVarStatus);
    Q_ASSERT(statusAdapter != nullptr);
    qDebug() << statusAdapter->content();
    
    this->showContextMenu();
}

void MainWindow::showContextMenu()
{
    auto statusAdapter = getStatusAdapterAtCurrentIndex();
    
    QMenu menu;
    /*
     * AUTHOR (user@instance.net)
     * --------------------------
     */
    QAction authorAction(statusAdapter->formattedAuthor());
    authorAction.setEnabled(false);
    menu.addAction(&authorAction);
    menu.addSeparator();
    
    
    // REPLY (&R)
    QAction replyAction(tr("REPLY (&R)"));
    QAction boostAction(tr("BOOST (&T)"));
    QAction favouriteAction(tr("FAVOURITE (&F)"));
    
    menu.addAction(&replyAction);
    menu.addAction(&boostAction);
    menu.addAction(&favouriteAction);
    
    if (statusAdapter->isBoosted()) {
        boostAction.setText(tr("UNBOOST (&T)"));
    }
    
    if (statusAdapter->isFavourited()) {
        favouriteAction.setText(tr("UNFAVOURITE (&F)"));
    }
    
    connect(&replyAction, &QAction::triggered, [=]() {
        ui->postArea->setReplyTo(statusAdapter);
        ui->postArea->focusPostArea();
    });
    
    connect(&boostAction, &QAction::triggered, [=]() {
        this->toggleBoost(statusAdapter);
    });
    
    connect(&favouriteAction, &QAction::triggered, [=]() {
        this->toggleFavourite(statusAdapter);
    });
    
    menu.exec(QCursor::pos());
}

MainWindow::~MainWindow()
{
}

