#pragma once

#include <QMainWindow>

#include "configmanager.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        void addAccount();
    signals:
        void quit();

    public slots:
        void updateTextLength(const QString& text);

    private:
        Ui::MainWindow *ui;
        ConfigManager *_configManager;
        QMenu *_lengthMenu;

        void initialize();
};
