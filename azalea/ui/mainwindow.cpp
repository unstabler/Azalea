#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>

#include <QQmlContext>
#include <QQuickItem>

#include "mastodon/apicontext.hpp"
#include "mastodon/mastodonapi.hpp"

#include "azaleaapplication.hpp"
#include "singleton.hpp"
#include "configmanager.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _timelineModel(),
    _configManager(new ConfigManager(this)),
    _apiContext(new APIContext(this))
{
    ui->setupUi(this);
    
    connect(this, &MainWindow::quit, this, &MainWindow::close, Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    // connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::updateTimeline);
    connect(ui->actionQuit,    &QAction::triggered, this, &MainWindow::quit);

    _configManager->load();
    if (_configManager->credentials()->length() <= 0) {
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
    
    if (_configManager->credentials()->empty()) {
        return;
    }
    
    auto defaultCredential = _configManager->credentials()->first();
    this->initializeWith(defaultCredential);
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
    
    for (auto timelineType : TimelineType::getAsIterable()) {
        _timelineModel[timelineType] = std::make_unique<TimelineModel>(
            new TimelineModel(this)
        );
    
        this->updateTimeline(timelineType);
    }
    
    tlViewContext->setContextProperty("timelineModel", _timelineModel[TimelineType::HOME].get());
}

void MainWindow::setTimeline(TimelineType::Enum timelineType)
{
    auto *tlViewContext = ui->timelineView->rootContext();
    tlViewContext->setContextProperty("timelineModel", _timelineModel[timelineType].get());
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
                "https://azalea.unstabler.pl"
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
            _configManager->credentials()->push_back(credential);
            _configManager->save();
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
    
    qDebug() << "performing refresh for timelineType " << timelineType;
    
    APIFutureResource<QList<v1::Status*>> *response = nullptr;
    
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
}

void MainWindow::timelineResolved(TimelineType::Enum timelineType, QSharedPointer<QList<v1::Status*>> statuses)
{
    auto *model = _timelineModel.at(timelineType).get();
    for (v1::Status *status : *statuses) {
        model->append(new StatusAdapter(this, status));
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
    qDebug() << (tr("keyPressEvent: %1").arg(event->key()));
    
    switch (event->key()) {
        case Qt::Key_U:
            ui->postArea->focusPostArea();
            break;
        case Qt::Key_Escape:
            ui->postArea->blurPostArea();
            break;
        case Qt::Key_J:
            setCurrentIndex(getCurrentIndex() + 1);
            break;
        case Qt::Key_K:
            setCurrentIndex(getCurrentIndex() - 1);
            break;
        default:
            goto default_handler;
    }
    
    return;
    
    default_handler:
    QWidget::keyPressEvent(event);
}

QQuickItem *MainWindow::getQMLTimeline()
{
    return ui->timelineView->rootObject()->findChild<QQuickItem *>("timeline");;
}

void MainWindow::setCurrentIndex(int index)
{
    getQMLTimeline()->setProperty("currentIndex", std::max(0, index));
}

int MainWindow::getCurrentIndex()
{
    return getQMLTimeline()->property("currentIndex").toInt();
}

MainWindow::~MainWindow()
{
    delete ui;
}


