#include "application.hpp"

template<class Archive>
void Application::serialize(Archive & archive)
{
    archive(
        cereal::make_nvp("id", id),
        cereal::make_nvp("client_id", clientId),
        cereal::make_nvp("client_secret", clientSecret)
    );
}
