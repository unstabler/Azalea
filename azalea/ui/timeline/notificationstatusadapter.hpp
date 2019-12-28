#pragma once

#include "mastodon/v1/entities/mastodon.hpp"
#include "statusadapterbase.hpp"

class NotificationStatusAdapter : public StatusAdapterBase
{
        Q_OBJECT
        
    public:
        NotificationStatusAdapter(QObject *parent, QSharedPointer<v1::Notification> notification);

        QString id() override;
        QString content() override;
        QString formattedAuthor() override;
        QString author() override;
        QUrl avatarUrl() override;
        QUrl interactAvatarUrl() override;
        QString createdAt() override;
        
        QStringList images() override;
        
        bool isBoosted() override;
        void setBoosted(bool isBoosted) override;
        bool isFavourited() override;
        void setFavourited(bool isFavourited) override;
        
        QSharedPointer<v1::Notification> notification();
        
    private:
        QSharedPointer<v1::Notification> _notification;
};

