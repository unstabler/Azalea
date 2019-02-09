#pragma once

#include <QString>

#include "serialization.hpp"

namespace oauth2 
{
    struct OAuthApplication 
    {
        public:
            QString id;
            QString redirectUri;
            QString clientId;
            QString clientSecret;
    };

    struct OAuthToken 
    {
        public:
            QString tokenType;
            QString accessToken;
            QString scope;
            qint64 createdAt;
            // TODO: 추후 timestamp는 QDate로 대체
    };
}

template<> void toJSON(oauth2::OAuthApplication *source, QJsonObject &destination);
template<> void fromJSON(oauth2::OAuthApplication *destination, QJsonObject source);
template<> void fromJSON(oauth2::OAuthToken *destination, QJsonObject source);