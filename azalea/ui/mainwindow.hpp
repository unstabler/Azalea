#pragma once

#include <memory>
#include <unordered_map>
#include <QMainWindow>
#include <ui/timeline/timelinemodel.hpp>
#include <mastodon/mastodonapi.hpp>
#include <QtQuick/QQuickItem>
#include <QtWidgets/QSystemTrayIcon>
#include <QQuickWidget>

#include "configmanager.hpp"

#include "mastodon/apicontext.hpp"
#include "mastodon/apibase.hpp"
#include "mastodon/v1/stream/streamingclient.hpp"
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
    
    static QString toString(Enum value) {
        static const auto resolver = [] (Enum value) {
            switch (value) {
                case HOME:
                    return "HOME";
                case MENTIONS:
                    return "MENTIONS";
                case DIRECT_MESSAGE:
                    return "DIRECT_MESSAGE";
                case INSTANCE_LOCAL:
                    return "INSTANCE_LOCAL";
                case INSTANCE_FEDERATED:
                    return "INSTANCE_FEDERATED";
            }
        };
        
        return QCoreApplication::translate("MainWindow", resolver(value));
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
        void initialize();

    public slots:
        void timelineResolved(TimelineType::Enum timelineType, QSharedPointer<ResourceList<v1::Status>> statuses);
        void notificationsResolved(QSharedPointer<ResourceList<v1::Notification>> notifications);
        void streamEvent(QString eventType, QJsonObject payload);
        
    private slots:
        void initializeShortcuts();
        void initializeTimelineTabs();
        
    
    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;


    private:
        Ui::MainWindow *ui;
        ConfigManager &_configManager;
        
        std::map<TimelineType::Enum, QShortcut*> _timelineShortcuts;
        
        APIContext *_apiContext;
        std::shared_ptr<MastodonAPI> _api;
        std::unique_ptr<StreamingClient> _streamingClient;
        std::map<TimelineType::Enum, std::unique_ptr<TimelineModel>> _timelineModel;
        std::map<TimelineType::Enum, QQuickWidget*> _timelineTabs;
        TimelineType::Enum _currentTimeline;
        
        qint64 _refreshKeyPressedAt = 0;
        
        
        void initializeWith(Credential *credential);
        void setTimeline(TimelineType::Enum timelineType);
        void updateTimeline(TimelineType::Enum timelineType, bool clear = false);
        void updateNotifications(bool clear = false);
        
        QQuickItem *getQMLTimeline();
        void setCurrentIndex(int index);
        int getCurrentIndex();
        StatusAdapterBase *getStatusAdapterAtCurrentIndex();
        
        void toggleBoost(StatusAdapterBase *statusAdapter);
        void toggleFavourite(StatusAdapterBase *statusAdapter);
        
};
