#pragma once

#include <QObject>
#include <mastodon/v1/entities/mastodon.hpp>

class StatusAdapterBase : public QObject
{
    Q_OBJECT;
    
    // TODO: Q_PROPERTY wrap
    Q_PROPERTY(QString content READ content FINAL CONSTANT);
    Q_PROPERTY(QString formattedAuthor READ formattedAuthor FINAL CONSTANT);
    Q_PROPERTY(QUrl avatarUrl READ avatarUrl FINAL CONSTANT);
    Q_PROPERTY(QUrl interactAvatarUrl READ interactAvatarUrl FINAL CONSTANT);
    Q_PROPERTY(QString createdAt READ createdAt FINAL CONSTANT);
    
public:
    explicit StatusAdapterBase(QObject *parent);
    
    virtual QString id() = 0;
    virtual QString content() = 0;
    
    // virtual QSharedPointer<v1::Account> account;
    // TODO: 이 아래로는 StatusAdapterBase로 다시 합쳐야 함
    virtual QString formattedAuthor() = 0;
    virtual QUrl avatarUrl() = 0;
    virtual QUrl interactAvatarUrl() = 0;
    virtual QString createdAt() = 0;
    
protected:
    QString formatAuthor(QSharedPointer<v1::Account> account);
};


