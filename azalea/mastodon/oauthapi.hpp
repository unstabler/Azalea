#pragma once

#include <cereal/cereal.hpp>
#include "apibase.hpp"

class OAuthAPI : public APIBase
{
public:
    explicit OAuthAPI(APIContext* context);

    QUrl getAuthorizeUrl(const QString applicationId, const QString scope);
};
