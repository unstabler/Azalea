#pragma once

#include <QSharedPointer>
#include <cereal/cereal.hpp>

#include "../apibase.hpp"
#include "./entities/application.hpp"

class AppsAPI : public APIBase
{
    public:
        static const QString NO_REDIRECT_URIS;

        explicit AppsAPI(APIContext* context);

        APIFutureResource<Application>* post(
            const QString clientName,
            const QString redirectUris,
            const QString scopes,
            const QString website
        );
};

