#pragma once

#include "application.hpp"

template<class Archive>
void serialize(Archive &archive, Application &application)
{
    archive(
        cereal::make_nvp("id", application.id),
        cereal::make_nvp("client_id", application.clientId),
        cereal::make_nvp("client_secret", application.clientSecret)
    );
}
