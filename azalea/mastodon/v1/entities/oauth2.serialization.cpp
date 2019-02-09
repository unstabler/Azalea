#include "oauth2.hpp"

using namespace deserialization;

template<> void toJSON(oauth2::OAuthApplication *source, QJsonObject &destination)
{
    destination["id"] = source->id;
    destination["redirect_uri"] = source->redirectUri;
    destination["client_id"] = source->clientId;
    destination["client_secret"] = source->clientSecret;
}

template<> void fromJSON(oauth2::OAuthApplication *destination, QJsonObject source) 
{
    destination->id = STRING(source["id"]);
    destination->redirectUri = STRING(source["redirect_uri"]);
    destination->clientId = STRING(source["client_id"]);
    destination->clientSecret = STRING(source["client_secret"]);
}

template<> void fromJSON(oauth2::OAuthToken *destination, QJsonObject source)
{
    destination->tokenType = STRING(source["token_type"]);
    destination->accessToken = STRING(source["access_token"]);
    destination->scope = STRING(source["scope"]);
    destination->createdAt = LONG(source["created_at"]);
}