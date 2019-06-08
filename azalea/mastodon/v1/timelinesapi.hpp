#pragma once

#include "../apibase.hpp"

#include "entities/mastodon.hpp"

namespace v1
{
    class TimelinesAPI : public APIBase
    {
        public:
            explicit TimelinesAPI(APIContext *context);
        
            APIFutureResource<QList<Status*>> *home(in::TimelinesAPIArgs &args);
            APIFutureResource<QList<Status*>> *local(in::TimelinesAPIArgs &args);
            APIFutureResource<QList<Status*>> *federated(in::TimelinesAPIArgs &args);
            APIFutureResource<QList<Status*>> *tag(const QString hashtag, in::TimelinesAPIArgs &args);

        protected:
            APIFutureResource<QList<Status*>> *_public(in::TimelinesAPIArgs &args, const bool local = false);
            APIFutureResource<QList<Status*>> *_timeline(const QString endpoint, QVariantMap params);
    };
}

