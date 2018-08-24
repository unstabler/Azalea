#pragma once

#include <QString>
#include <cereal/cereal.hpp>

struct OAuthToken
{
    QString tokenType;
    QString accessToken;
    QString scope;
    long createdAt;
};
