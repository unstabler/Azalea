#pragma once

#include <QDateTime>
#include <QString>
#include <QList>
#include <QUrl>

#include "serialization.hpp"
#include "optional.hpp"

namespace v1 
{
    enum StatusVisibility 
    {
        PUBLIC, UNLISTED, PRIVATE, DIRECT
    };

    struct Emoji 
    {
        QString shortcode;
        QUrl staticUrl;
        QUrl url;
        bool visibleInPicker;
    };

    struct Account
    {
        QString id;
        QString username;
        QString acct;
        QString displayName;
        bool locked;
        
        
        QString avatar;
        QString avatarStatic;
    };

    struct Mention 
    {
        public:
            QString url;
            QString username;
            QString acct;
            QString id;
    };

    struct Tag 
    {
        QString name;
        QString url;
    };

    /**
     * TODO: operator= override
     */
    struct Status 
    {
        public:
            QString id;
            QString uri;
            QString url;

            QSharedPointer<Account> account;
            QString inReplyToId;
            QString inReplyToAccountId;
            
            QSharedPointer<Status> reblog;
            QString content;

            QDateTime createdAt;
            QList<QSharedPointer<Emoji>> emojis;

            uint repliesCount;
            uint reblogsCount;
            uint favoritesCount;

            bool reblogged;
            bool favourited;
            bool muted;
            bool sensitive;

            QString spoilerText;
            StatusVisibility visibility;

            // TODO: attachments
            QList<Mention*> mentions;
            QList<Tag*> tags;

            bool pinned;
    };
    
    struct Notification {
        QString id;
        QString type;
        QDateTime createdAt;
        QSharedPointer<Account> account;
        QSharedPointer<Status> status;
    };
    
    namespace in
    {
        struct TimelinesAPIArgs
        {
            Optional<QString> maxId;
            Optional<QString> sinceId;
            Optional<QString> minId;
            Optional<QString> limit;
        };
        
        struct NotificationListArgs
        {
            Optional<QString> maxId;
            Optional<QString> sinceId;
            Optional<QString> minId;
            Optional<QString> limit;
            Optional<QString> excludeTypes;
            Optional<QString> accountId;
        };
        
        struct PostStatusArgs
        {
            Optional<QString> status;
            Optional<QString> inReplyToId;
            Optional<QList<QString>> mediaIds;
            Optional<bool> sensitive;
            Optional<QString> spoilerText;
            Optional<QString> visibility;
        };
    }
}

// TODO: template <X> fromJSON( X* destination, JsonObject* source... ) 만들기!!!!
// 힘내자!

template<> void fromJSON(v1::Account *destination, QJsonObject source);
template<> void fromJSON(v1::Emoji *destination, QJsonObject source);
template<> void fromJSON(v1::Status *destination, QJsonObject source);
template<> void fromJSON(v1::Notification *destination, QJsonObject source);

template<> void toJSON(v1::in::PostStatusArgs *source, QJsonObject &destination);
template<> void toJSON(v1::in::TimelinesAPIArgs *source, QJsonObject &destination);
template<> void toJSON(v1::in::NotificationListArgs *source, QJsonObject &destination);
