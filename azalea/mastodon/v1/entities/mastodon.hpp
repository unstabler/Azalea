#pragma once

#include <QDateTime>
#include <QString>
#include <QList>

#include "serialization.hpp"

namespace v1 
{
    enum StatusVisibility 
    {
        PUBLIC, UNLISTED, PRIVATE, DIRECT
    };

    struct Emoji 
    {

    };

    struct Account
    {

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

            Account* account;
            QString inReplyToId;
            QString inReplyToAccountId;
            
            Status* reblog;
            QString content;

            QDateTime createdAt;
            QList<Emoji*> emojis;

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
}

// TODO: template <X> fromJSON( X* destination, JsonObject* source... ) 만들기!!!!
// 힘내자!

template<> void fromJSON(v1::Account *destination, QJsonObject source);
template<> void fromJSON(v1::Status *destination, QJsonObject source);