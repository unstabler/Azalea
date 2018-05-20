#pragma once

#include <QString>
#include <cereal/cereal.hpp>

struct Application
{
        QString id;
        QString clientId;
        QString clientSecret;

        template<class Archive>
        void serialize(Archive& archive);
};
