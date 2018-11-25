#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>

#include "mastodon/apicontext.hpp"
#include "mastodon/mastodonapi.hpp"

#include "azaleaapplication.hpp"
#include "singleton.hpp"
#include "configmanager.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _configManager(new ConfigManager(this))
{
    ui->setupUi(this);
    _lengthMenu = ui->menuBar->addMenu(tr("0"));

    connect(ui->simpleTootEdit, &QLineEdit::textChanged, this, &MainWindow::updateTextLength);
    connect(this, &MainWindow::quit, this, &MainWindow::close, Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));

    _configManager->load();
    if (_configManager->token().isEmpty())
    {
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
        qDebug() << QString("created application #%1").arg(
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

void MainWindow::updateTextLength(const QString &text)
{
    auto lengthLeft = 500 - text.length();
    _lengthMenu->setTitle(QString::number(lengthLeft));
}

MainWindow::~MainWindow()
{
    delete ui;
}
