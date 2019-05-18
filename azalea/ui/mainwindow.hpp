#pragma once

#include <QMainWindow>
#include <ui/timeline/TimelineModel.hpp>

#include "configmanager.hpp"

#include "mastodon/apicontext.hpp"
#include "mastodon/apibase.hpp"
#include "postarea.hpp"

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
        void updateTimeline();

    private:
        Ui::MainWindow *ui;
        ConfigManager *_configManager;
        QMenu *_lengthMenu;
        
        APIContext *_apiContext;
        TimelineModel _timelineModel;

        void initialize();
};
