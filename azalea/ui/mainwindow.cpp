#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "../mastodon/apicontext.hpp"
#include "../mastodon/mastodonapi.hpp"
#include "serialization.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    APIContext* context = new APIContext();
    context->setHost("twingyeo.kr");
    MastodonAPI* api = new MastodonAPI(context);
    auto* futureApplication = api->apps()->post(
        "Azalea",
        v1::AppsAPI::NO_REDIRECT_URIS,
        "read",
        "https://azalea.unstabler.pl"
    );
    connect(futureApplication, &APIFutureResponse::resolved, [=]() {
        auto application = futureApplication->tryDeserialize();
        qDebug() << application->id;
        qDebug() << application->clientId;
        qDebug() << application->clientSecret;
        qDebug() << "OK!";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
