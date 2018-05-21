#pragma once

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:
        void updateTextLength(const QString& text);

    private:
        Ui::MainWindow *ui;
        QMenu* lengthMenu;
};
