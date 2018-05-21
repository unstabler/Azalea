#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QMenuBar>

#include "../mastodon/apicontext.hpp"
#include "../mastodon/mastodonapi.hpp"
#include "serialization.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lengthMenu = ui->menuBar->addMenu(tr("0"));

    connect(ui->simpleTootEdit, &QLineEdit::textChanged, this, &MainWindow::updateTextLength);
}

void MainWindow::updateTextLength(const QString& text)
{
    auto lengthLeft = 500 - text.length();
    lengthMenu->setTitle(QString::number(lengthLeft));
}

MainWindow::~MainWindow()
{
    delete ui;
}
