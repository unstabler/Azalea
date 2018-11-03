#pragma once

#include "apibase.hpp"

#include "oauth2.pb.h"

class OAuthAPI : public APIBase
{
public:
    explicit OAuthAPI(APIContext* context);

    QUrl getAuthorizeUrl(const QString applicationId, const QString scope);

    APIFutureResource<v1::OAuthToken>* tokenByAuthorizationCode(
        const QString clientId,
        const QString clientSecret,
        const QString code
    );
};
