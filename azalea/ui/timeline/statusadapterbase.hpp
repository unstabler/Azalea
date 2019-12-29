#pragma once

#include <QObject>
#include <mastodon/v1/entities/mastodon.hpp>

class StatusAdapterBase : public QObject
{
    Q_OBJECT;
    // TODO: DISABLE COPY
    
    // TODO: Q_PROPERTY wrap
    Q_PROPERTY(QString content READ content FINAL CONSTANT);
    Q_PROPERTY(QString formattedAuthor READ formattedAuthor FINAL CONSTANT);
    Q_PROPERTY(QString author READ author FINAL CONSTANT);
    Q_PROPERTY(QUrl avatarUrl READ avatarUrl FINAL CONSTANT);
    Q_PROPERTY(QUrl interactAvatarUrl READ interactAvatarUrl FINAL CONSTANT);
    Q_PROPERTY(QString createdAt READ createdAt FINAL CONSTANT);
    
    Q_PROPERTY(QStringList images READ images FINAL CONSTANT);
    
    Q_PROPERTY(bool boosted READ isBoosted WRITE setBoosted NOTIFY boostToggled);
    Q_PROPERTY(bool favourited READ isFavourited WRITE setFavourited NOTIFY favouriteToggled);
    
public:
    explicit StatusAdapterBase(QObject *parent);
    
    virtual QString id() = 0;
    virtual QString content() = 0;
    
    // virtual QSharedPointer<v1::Account> account;
    // TODO: 이 아래로는 StatusAdapterBase로 다시 합쳐야 함
    virtual QString formattedAuthor() = 0;
    virtual QString author() = 0;
    virtual QUrl avatarUrl() = 0;
    virtual QUrl interactAvatarUrl() = 0;
    virtual QString createdAt() = 0;
    
    virtual QStringList images() = 0;
    
    virtual bool isBoosted() = 0;
    virtual void setBoosted(bool isBoosted) = 0;
    
    virtual bool isFavourited() = 0;
    virtual void setFavourited(bool isFavourited) = 0;
    
    virtual QUrl statusUrl() = 0;
    
signals:
    void boostToggled(bool isBoosted);
    void favouriteToggled(bool isFavourited);
    
protected:
    QString formatAuthor(QSharedPointer<v1::Account> account);
    QStringList filterAttachmentUrls(QSharedPointer<v1::Status> status, const QString type);
};


