#pragma once

#include "../apibase.hpp"

#include "entities/mastodon.hpp"

namespace v1
{
    class TimelinesAPI : public APIBase
    {
        public:
            explicit TimelinesAPI(APIContext *context);
        
            APIFutureResource<ResourceList<Status>> *home(in::TimelinesAPIArgs &args);
            APIFutureResource<ResourceList<Status>> *local(in::TimelinesAPIArgs &args);
            APIFutureResource<ResourceList<Status>> *federated(in::TimelinesAPIArgs &args);
            APIFutureResource<ResourceList<Status>> *tag(const QString hashtag, in::TimelinesAPIArgs &args);

        protected:
            APIFutureResource<ResourceList<Status>> *_public(in::TimelinesAPIArgs &args, const bool local = false);
            APIFutureResource<ResourceList<Status>> *_timeline(const QString endpoint, QVariantMap argsMap);
    };
}

