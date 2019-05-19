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
    
    connect(ui->tootEdit, &QTextEdit::textChanged, this, &PostArea::textChanged);
    connect(ui->postButton, &QPushButton::clicked, this, &PostArea::submitPost);
    
    connect(this->_postShortcut, &QShortcut::activated, this, &PostArea::submitPost);
    
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


void PostArea::submitPost()
{
    // TODO;
    qDebug() << "TODO: submit post";
    this->ui->tootEdit->setText("");
}
