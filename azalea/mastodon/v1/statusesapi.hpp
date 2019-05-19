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
        
    };
}


