#pragma once

#include <QObject>
#include <QList>

#include "mastodon/v1/entities/mastodon.hpp"
#include "statusadapterbase.hpp"

class StatusAdapter : public StatusAdapterBase
{
public:
    StatusAdapter(QObject *parent, QSharedPointer<v1::Status> status);
    
    QString id() override;
    QString content() override;
    QString formattedAuthor() override;
    QUrl avatarUrl() override;
    QUrl interactAvatarUrl() override;
    QString createdAt() override;
    
    QSharedPointer<v1::Status> status();
    
private:
    QSharedPointer<v1::Status> _status;
    QList<StatusAdapter*> _replies;
};
