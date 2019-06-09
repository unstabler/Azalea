#pragma once

#include <memory>
#include <unordered_map>
#include <QMainWindow>
#include <ui/timeline/TimelineModel.hpp>
#include <mastodon/mastodonapi.hpp>
#include <QtQuick/QQuickItem>

#include "configmanager.hpp"

#include "mastodon/apicontext.hpp"
#include "mastodon/apibase.hpp"
#include "postarea.hpp"

namespace Ui {
    class MainWindow;
}

namespace TimelineType {
    enum Enum {
        HOME = 1,
        MENTIONS = 2,
        DIRECT_MESSAGE = 3,
        INSTANCE_LOCAL = 4,
        INSTANCE_FEDERATED = 5
    };
    
    static std::array<Enum, 5> getAsIterable() {
        return {
            HOME,
            MENTIONS,
            DIRECT_MESSAGE,
            INSTANCE_LOCAL,
            INSTANCE_FEDERATED
        };
    }
};


class MainWindow : public QMainWindow
{
        Q_OBJECT
        Q_DISABLE_COPY(MainWindow)

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        void addAccount();
        std::shared_ptr<MastodonAPI> api() const;
    signals:
        void quit();

    public slots:
        void timelineResolved(TimelineType::Enum timelineType, QSharedPointer<QList<v1::Status*>> statuses);
    
    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;


private:
        Ui::MainWindow *ui;
        ConfigManager *_configManager;
        
        std::map<TimelineType::Enum, QShortcut*> _timelineShortcuts;
        
        APIContext *_apiContext;
        std::shared_ptr<MastodonAPI> _api;
        std::map<TimelineType::Enum, std::unique_ptr<TimelineModel>> _timelineModel;
        TimelineType::Enum _currentTimeline;
        
        qint64 _refreshKeyPressedAt = 0;
        
        
        void initializeShortcuts();
        
        void initializeWith(Credential *credential);
        void setTimeline(TimelineType::Enum timelineType);
        void updateTimeline(TimelineType::Enum timelineType, bool clear = false);
        
        QQuickItem *getQMLTimeline();
        void setCurrentIndex(int index);
        int getCurrentIndex();
};
