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
    QUrl avatarUrl() override;
    QString createdAt() override;
    
    QSharedPointer<v1::Notification> notification();
    
private:
    QSharedPointer<v1::Notification> _notification;
};

