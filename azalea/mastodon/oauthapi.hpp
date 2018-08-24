#pragma once

#include <cereal/cereal.hpp>

#include "apibase.hpp"
#include "oauthtoken.hpp"

class OAuthAPI : public APIBase
{
public:
    explicit OAuthAPI(APIContext* context);

    QUrl getAuthorizeUrl(const QString applicationId, const QString scope);

    APIFutureResource<OAuthToken>* tokenByAuthorizationCode(
        const QString clientId,
        const QString clientSecret,
        const QString code
    );
};
