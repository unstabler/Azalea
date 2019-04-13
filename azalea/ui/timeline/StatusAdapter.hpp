#pragma once

#include <QObject>
#include <QList>

#include "mastodon/v1/entities/mastodon.hpp"

class StatusAdapter : public QObject
{
    Q_OBJECT
    // TODO: Q_PROPERTY wrap
    Q_PROPERTY(QString content READ content FINAL);
    Q_PROPERTY(QString formattedAuthor READ formattedAuthor FINAL);
    Q_PROPERTY(QUrl avatarUrl READ avatarUrl FINAL);

public:
    StatusAdapter(QObject *parent, v1::Status *status);
    
    QString content();
    QString formattedAuthor();
    QUrl avatarUrl();
    
private:
        v1::Status *_status;
        QList<StatusAdapter*> _replies;
};
