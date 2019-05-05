#pragma once

#include <QObject>
#include <QList>

#include "mastodon/v1/entities/mastodon.hpp"

class StatusAdapter : public QObject
{
    Q_OBJECT
    // TODO: Q_PROPERTY wrap
    Q_PROPERTY(QString content READ content FINAL CONSTANT);
    Q_PROPERTY(QString formattedAuthor READ formattedAuthor FINAL CONSTANT);
    Q_PROPERTY(QUrl avatarUrl READ avatarUrl FINAL CONSTANT);
    Q_PROPERTY(QString createdAt READ createdAt FINAL CONSTANT);

public:
    StatusAdapter(QObject *parent, v1::Status *status);
    
    QString content();
    QString formattedAuthor();
    QUrl avatarUrl();
    QString createdAt();
    
private:
        v1::Status *_status;
        QList<StatusAdapter*> _replies;
};
