#pragma once

#include <memory>
#include <array>
#include <unordered_map>
#include <QMainWindow>
#include <QtQuick/QQuickItem>
#include <QtWidgets/QSystemTrayIcon>
#include <QQuickWidget>
#include <QMenu>

#include "configmanager.hpp"

#include "mastodon/mastodonapi.hpp"
#include "mastodon/apicontext.hpp"
#include "mastodon/apibase.hpp"
#include "mastodon/v1/stream/streamingclient.hpp"
#include "postarea.hpp"
#include "timeline/timelinemodel.hpp"

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
        switch (value) {
            case HOME:
                return QCoreApplication::translate("MainWindow", "HOME");
            case MENTIONS:
                return QCoreApplication::translate("MainWindow", "MENTIONS");
            case DIRECT_MESSAGE:
                return QCoreApplication::translate("MainWindow", "DIRECT_MESSAGE");
            case INSTANCE_LOCAL:
                return QCoreApplication::translate("MainWindow", "INSTANCE_LOCAL");
            case INSTANCE_FEDERATED:
                return QCoreApplication::translate("MainWindow", "INSTANCE_FEDERATED");
        }
        
        throw std::runtime_error("unexpected enum value");
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
        void initialized();

    public slots:
        void timelineResolved(TimelineType::Enum timelineType, QSharedPointer<ResourceList<v1::Status>> statuses);
        void notificationsResolved(QSharedPointer<ResourceList<v1::Notification>> notifications);
        void streamEvent(QString eventType, QJsonObject payload);
        
    private slots:
        void initializeShortcuts();
        void initializeTimelineTabs();
        
        void onQMLTimelineRightClicked(const QVariant &qVarStatus);
        void showContextMenu();
        
    
    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;


    private:
        QScopedPointer<Ui::MainWindow> ui;
        ConfigManager &_configManager;
        
        std::map<TimelineType::Enum, QShortcut*> _timelineShortcuts;
        
        APIContext &_apiContext;
        [[deprecated]]
        QScopedPointer<MastodonAPI> _api;
        [[deprecated]]
        QScopedPointer<StreamingClient> _streamingClient;
        std::map<TimelineType::Enum, std::unique_ptr<TimelineModel>> _timelineModel;
        std::map<TimelineType::Enum, QQuickWidget*> _timelineTabs;
        TimelineType::Enum _currentTimeline;
        
        QMenu _tootContextMenu;
        
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
