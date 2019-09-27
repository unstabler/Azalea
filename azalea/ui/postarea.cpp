#include <mastodon/mastodonapi.hpp>
#include "postarea.hpp"
#include "ui_postarea.h"

#include "mainwindow.hpp"

PostArea::PostArea(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PostArea),
    _maxPostLength(500),
    _replyTo(nullptr)
{
    ui->setupUi(this);
    ui->retranslateUi(this);
    
    ui->replyIndicator->setVisible(false);
    
    this->_postShortcut = new QShortcut((Qt::CTRL + Qt::Key_Return), ui->tootEdit);
    this->_postShortcut->setContext(Qt::WidgetShortcut);
    this->_postShortcut->setAutoRepeat(false);
    this->_postShortcut->setEnabled(true);
    
    connect(ui->tootEdit, &QTextEdit::textChanged, this, &PostArea::textChanged);
    connect(ui->postButton, &QPushButton::clicked, this, &PostArea::submitPost);
    connect(ui->replyIndicator, &ReplyIndicator::cancel, this, [this] { this->setReplyTo(nullptr); });
    
    connect(this->_postShortcut, &QShortcut::activated, this, &PostArea::submitPost);
    
    
    this->updatePostLength();
}

PostArea::~PostArea()
{
    delete ui;
}

bool PostArea::isFocused()
{
    return ui->tootEdit->hasFocus() || ui->postButton->hasFocus();
}

void PostArea::setReplyTo(StatusAdapterBase *replyTo)
{
    _replyTo = replyTo;
    ui->replyIndicator->setReplyTo(replyTo);
    
    if (replyTo != nullptr) {
        ui->replyIndicator->setVisible(true);
        this->setText(replyTo->author() + " ");
    } else {
        ui->replyIndicator->setVisible(false);
    }
}

void PostArea::setText(QString text)
{
    ui->tootEdit->setText(text);
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
    if (this->_replyTo != nullptr) {
        args.inReplyToId.set(this->_replyTo->id());
    }
    qDebug() << args.status.get();
    
    
    this->ui->tootEdit->setEnabled(false);
    
    auto response = api->statuses()->post(args);
    connect(response, &APIFutureResponse::resolved, [this, response]() {
        auto status = response->tryDeserialize();
        this->ui->tootEdit->setEnabled(true);
    });
    connect(response, &APIFutureResponse::rejected, [this, response](int code, QString content) {
        qDebug() << code << content;
        
        this->ui->tootEdit->setEnabled(true);
    });
    
    this->ui->tootEdit->setText("");
    this->setReplyTo(nullptr);
}

void PostArea::focusPostArea()
{
    ui->tootEdit->setFocus(Qt::ShortcutFocusReason);
    
    // TODO: optional parameter로 설정ㅈ할 수 있게 해야 한다
    auto cursor = ui->tootEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->tootEdit->setTextCursor(cursor);
}


void PostArea::blurPostArea()
{
    if (ui->tootEdit->hasFocus()) {
        ui->tootEdit->clearFocus();
    }
}


