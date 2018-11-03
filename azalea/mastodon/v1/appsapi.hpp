#pragma once

#include <QSharedPointer>

#include "../apibase.hpp"
#include "oauth2.pb.h" 

namespace v1
{
    class AppsAPI : public APIBase
    {
        public:
            static const QString NO_REDIRECT_URIS;

            explicit AppsAPI(APIContext* context);

            APIFutureResource<OAuthApplication>* post(
                const QString clientName,
                const QString redirectUris,
                const QString scopes,
                const QString website
            );
    };
}

