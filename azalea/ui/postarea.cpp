#include <mastodon/mastodonapi.hpp>
#include "postarea.hpp"
#include "ui_postarea.h"

#include "mainwindow.hpp"

PostArea::PostArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PostArea),
    _maxPostLength(500)
{
    ui->setupUi(this);
    
    this->_postShortcut = new QShortcut((Qt::CTRL + Qt::Key_Return), ui->tootEdit);
    this->_postShortcut->setContext(Qt::WidgetShortcut);
    this->_postShortcut->setAutoRepeat(false);
    this->_postShortcut->setEnabled(true);
    
    this->_focusShortcut = new QShortcut(Qt::Key_U, getMainWindow());
    this->_blurFocusShortcut = new QShortcut(Qt::Key_Escape, ui->tootEdit);
    
    connect(ui->tootEdit, &QTextEdit::textChanged, this, &PostArea::textChanged);
    connect(ui->postButton, &QPushButton::clicked, this, &PostArea::submitPost);
    
    connect(this->_postShortcut, &QShortcut::activated, this, &PostArea::submitPost);
    connect(this->_focusShortcut, &QShortcut::activated, this, &PostArea::focusPostArea);
    connect(this->_blurFocusShortcut, &QShortcut::activated, this, &PostArea::blurPostArea);
    
    this->updatePostLength();
}

PostArea::~PostArea()
{
    delete ui;
}

MainWindow *PostArea::getMainWindow()
{
    return dynamic_cast<MainWindow*>(parentWidget()->window());
}

void PostArea::textChanged()
{
    updatePostLength();
}

void PostArea::setMaxPostLength(unsigned maxPostLength)
{
    this->_maxPostLength = maxPostLength;
    this->updatePostLength();
}

void PostArea::updatePostLength()
{
    this->_postLength = this->_maxPostLength - this->ui->tootEdit->toPlainText().length();
    this->ui->lengthLabel->setText(QString("%1").arg(this->_postLength));
}


void PostArea::submitPost()
{
    auto api = getMainWindow()->api();
    
    v1::in::PostStatusArgs args;
    args.status.set(this->ui->tootEdit->toPlainText());
    qDebug() << args.status.get();
    
    
    this->ui->tootEdit->setEnabled(false);
    
    auto response = api->statuses()->post(args);
    connect(response, &APIFutureResponse::resolved, [&]() {
        auto status = response->tryDeserialize();
        this->ui->tootEdit->setEnabled(true);
    });
    connect(response, &APIFutureResponse::rejected, [&](int code, QString content) {
        qDebug() << code << content;
        
        this->ui->tootEdit->setEnabled(true);
    });
    
    this->ui->tootEdit->setText("");
}

void PostArea::focusPostArea()
{
    ui->tootEdit->setFocus(Qt::ShortcutFocusReason);
}


void PostArea::blurPostArea()
{
    ui->tootEdit->clearFocus();
}
