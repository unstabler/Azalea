#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "../mastodon/apicontext.hpp"
#include "../mastodon/mastodonapi.hpp"
#include "../mastodon/apibase.hpp"
#include "../mastodon/v1/appsapi.hpp"

// FIXME: 이거 대체 뭔데 C++ 코드를 include 하지 않으면 빌드가 안되는걸까..
#include "../mastodon/v1/entities/application.cpp"
#include "../mastodon/v1/entities/application.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    APIContext* context = new APIContext();
    context->setHost("twingyeo.kr");
    AppsAPI* api = new AppsAPI(context);
    auto* futureApplication = api->post(
        "Azalea",
        AppsAPI::NO_REDIRECT_URIS,
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
