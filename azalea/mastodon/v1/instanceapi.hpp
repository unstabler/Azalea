#pragma once

#include "../apibase.hpp"

#include "entities/mastodon.hpp"
#include "entities/oauth2.hpp"

namespace v1
{
    class InstanceAPI : public APIBase
    {
        public:
            explicit InstanceAPI(APIContext *context);
            APIFutureResource<ResourceList<Emoji>> *getCustomEmojiList();
    };
}

