#pragma once

#include <QSharedPointer>

#include "../apibase.hpp"

#include "entities/mastodon.hpp"
#include "entities/oauth2.hpp"

namespace v1
{
    class AppsAPI : public APIBase
    {
        public:
            static const QString NO_REDIRECT_URIS;

            explicit AppsAPI(APIContext *context);

            APIFutureResource<oauth2::OAuthApplication>* post(
                const QString clientName,
                const QString redirectUris,
                const QString scopes,
                const QString website
            );
    };
}

