#pragma once

#include <memory>
#include <QMainWindow>
#include <ui/timeline/TimelineModel.hpp>
#include <mastodon/mastodonapi.hpp>

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
        std::shared_ptr<MastodonAPI> api() const;
    signals:
        void quit();

    public slots:
        void updateTimeline();

    private:
        Ui::MainWindow *ui;
        ConfigManager *_configManager;
        
        APIContext *_apiContext;
        std::shared_ptr<MastodonAPI> _api;
        TimelineModel _timelineModel;

        void initialize();
};
