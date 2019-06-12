//
// Created by cheesekun on 19/06/11.
//

#ifndef AZALEA_SETTINGSWINDOW_HPP
#define AZALEA_SETTINGSWINDOW_HPP

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <configmanager.hpp>
#include <singleton.hpp>

namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(SettingsWindow)
    
public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();
    
private:
    Ui::SettingsWindow *ui;
    
    QLabel *_label;
    ConfigManager &_configManager;
    
};

#endif //AZALEA_SETTINGSWINDOW_HPP
