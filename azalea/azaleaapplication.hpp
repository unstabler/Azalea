#pragma once

#include <QObject>
#include <QApplication>

#include "ui/mainwindow.hpp"

/**
 * @brief Azalea 애플리케이션 클래스
 */
class AzaleaApplication : public QApplication
{
public:
    static const QString APPLICATION_NAME;
    static const QString APPLICATION_VERSION;

    AzaleaApplication(int argc, char* argv[]);

    int exec();

private:
    MainWindow* mainWindow;

};
