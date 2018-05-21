#pragma once

#include <QString>
#include <cereal/cereal.hpp>

namespace v1 {
    struct Application
    {
            QString id;
            QString clientId;
            QString clientSecret;
    };
}
