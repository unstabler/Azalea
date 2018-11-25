#pragma once

#include "apibase.hpp"

#include "v1/entities/oauth2.hpp"

class OAuthAPI : public APIBase
{
public:
    explicit OAuthAPI(APIContext* context);

    QUrl getAuthorizeUrl(const QString applicationId, const QString scope);

    APIFutureResource<oauth2::OAuthToken>* tokenByAuthorizationCode(
        const QString clientId,
        const QString clientSecret,
        const QString code
    );
};
