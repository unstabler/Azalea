//
// Created by cheesekun on 19/06/11.
//

#include <QCheckBox>
#include "ui_settingswindow.h"

#include "settingswindow.hpp"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow),
    _configManager(Singleton<ConfigManager>::getInstance())
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{

}
