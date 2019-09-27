//
// Created by cheesekun on 2019/09/12.
//
#include "ui_replyindicator.h"

#include "replyindicator.hpp"

ReplyIndicator::ReplyIndicator(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ReplyIndicator)
{
    ui->setupUi(this);
    ui->retranslateUi(this);
    
    connect(ui->cancelButton, &QPushButton::pressed, this, [this] {emit cancel();});
}

ReplyIndicator::~ReplyIndicator()
{
    delete ui;
}

void ReplyIndicator::setReplyTo(StatusAdapterBase *replyTo)
{
    this->_replyTo = replyTo;
    if (replyTo != nullptr) {
        ui->content->setText(_replyTo->content());
    }
}