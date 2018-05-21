#include "postarea.hpp"
#include "ui_postarea.h"

PostArea::PostArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PostArea)
{
    ui->setupUi(this);
}

PostArea::~PostArea()
{
    delete ui;
}
