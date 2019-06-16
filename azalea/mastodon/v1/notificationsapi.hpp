#pragma once

#include "../apibase.hpp"

#include "entities/mastodon.hpp"

namespace v1
{
    class NotificationsAPI : public APIBase
    {
        public:
            explicit NotificationsAPI(APIContext *context);
            
            APIFutureResource<ResourceList<Notification>> *list(v1::in::NotificationListArgs &args);
    };
}
