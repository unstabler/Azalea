//
// Created by cheesekun on 19/05/19.
//
#pragma once

#include "../apibase.hpp"

#include "entities/mastodon.hpp"

namespace v1
{
    class StatusesAPI : public APIBase
    {
        public:
            explicit StatusesAPI(APIContext *context);
            
            APIFutureResource<Status> *post(in::PostStatusArgs &args);
            
            APIFutureResource<Status> *reblog(const QString statusId);
            APIFutureResource<Status> *unreblog(const QString statusId);
            APIFutureResource<Status> *favourite(const QString statusId);
            APIFutureResource<Status> *unfavourite(const QString statusId);
            
    };
}


