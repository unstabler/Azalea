//
// Created by cheesekun on 19/05/19.
//

#include "statusesapi.hpp"

namespace v1
{
    StatusesAPI::StatusesAPI(APIContext *context) :
            APIBase(context)
    {
    
    }
    
    APIFutureResource<Status> *StatusesAPI::post(in::PostStatusArgs &args)
    {
        static const QString ENDPOINT = "/api/v1/statuses";
        auto argsMap = toJSON(&args).toVariantMap();
        
        QNetworkReply *reply = this->POST(ENDPOINT, argsMap);
        return new APIFutureResource<Status>(reply);
    }
}