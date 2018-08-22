#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>

#include "mastodon/apicontext.hpp"
#include "mastodon/mastodonapi.hpp"

#include "singleton.hpp"
#include "configmanager.hpp"
#include "serialization.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _configManager(new ConfigManager(this))
{
    ui->setupUi(this);
    _lengthMenu = ui->menuBar->addMenu(tr("0"));

    connect(ui->simpleTootEdit, &QLineEdit::textChanged, this, &MainWindow::updateTextLength);

    _configManager->load();
    if (_configManager->token().isEmpty())
    {
        auto decision = QMessageBox::question(
                    this,
                    this->windowTitle(),
                    tr("NO_TOKEN_SET")
        );

        if (decision == QMessageBox::Yes)
        {
            auto instanceName = QInputDialog::getText(
                        this,
                        tr("INSTANCE_NAME"),
                        tr("INPUT_INSTANCE_NAME")
            );

            qDebug() << instanceName;
            auto context = new APIContext(this);
            context->setHost(instanceName);
            auto api = new MastodonAPI(context);
            auto response = api->apps()->post(
                        "Azalea",
                        v1::AppsAPI::NO_REDIRECT_URIS,
                        "read",
                        "https://azalea.unstabler.pl"
            );
            connect(response, &APIFutureResponse::resolved, [=]() {
                auto application = response->tryDeserialize();
                qDebug() << application->id;
                qDebug() << application->clientId;
                qDebug() << application->clientSecret;
            });
        }
    }
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
