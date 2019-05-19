#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>

#include <QQmlContext>

#include "mastodon/apicontext.hpp"
#include "mastodon/mastodonapi.hpp"

#include "azaleaapplication.hpp"
#include "singleton.hpp"
#include "configmanager.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _timelineModel(this),
    _configManager(new ConfigManager(this)),
    _apiContext(new APIContext(this))
{
    ui->setupUi(this);

    connect(this, &MainWindow::quit, this, &MainWindow::close, Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);

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
    
    
    
    auto *qContext = ui->timelineView->rootContext();
    qContext->setContextProperty("timelineModel", &_timelineModel);
    
    ui->timelineView->setSource(QUrl(QStringLiteral("qrc:/components/Timeline.qml")));
    
    if (_configManager->credentials()->empty()) {
        return;
    }
    auto defaultCredential = _configManager->credentials()->first();
    
    _apiContext->setHost(defaultCredential->instanceName());
    _apiContext->setToken(defaultCredential->token()->accessToken);
    this->updateTimeline();
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

void MainWindow::updateTimeline()
{
    v1::in::TimelinesAPIArgs args;
    auto api = new MastodonAPI(_apiContext);
    auto response = api->timelines()->home(args);
    connect(response, &APIFutureResponse::resolved, [=]() {
        auto timeline = response->tryDeserialize();
        for (auto status : *timeline) {
            _timelineModel.append(new StatusAdapter(this, status));
        }
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}

