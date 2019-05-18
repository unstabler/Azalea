#include "postarea.hpp"
#include "ui_postarea.h"

#include "mainwindow.hpp"

PostArea::PostArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PostArea),
    _maxPostLength(500)
{
    ui->setupUi(this);
    
    connect(ui->tootEdit, &QTextEdit::textChanged, this, &PostArea::textChanged);
    this->updatePostLength();
}

PostArea::~PostArea()
{
    delete ui;
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

MainWindow *PostArea::mainWindow()
{
    return dynamic_cast<MainWindow*>(this->parentWidget());
}
