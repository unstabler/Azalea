#pragma once

#include "oauthtoken.hpp"

template <class Archive>
void serialize(Archive &archive, OAuthToken &token) {
    archive(
        cereal::make_nvp("token_type", token.tokenType),
        cereal::make_nvp("access_token", token.accessToken),
        cereal::make_nvp("scope", token.scope),
        cereal::make_nvp("created_at", token.createdAt)
    );
}
