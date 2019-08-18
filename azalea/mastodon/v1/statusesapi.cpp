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
    
    APIFutureResource<Status> *StatusesAPI::reblog(const QString statusId)
    {
        const QString endpoint = QString("/api/v1/statuses/%1/reblog").arg(statusId);
        
        auto *reply = this->POST(endpoint, QVariantMap());
        return new APIFutureResource<Status>(reply);
    }
    
    APIFutureResource<Status> *StatusesAPI::unreblog(const QString statusId)
    {
        const QString endpoint = QString("/api/v1/statuses/%1/unreblog").arg(statusId);
    
        auto *reply = this->POST(endpoint, QVariantMap());
        return new APIFutureResource<Status>(reply);
    }
    
    APIFutureResource<Status> *StatusesAPI::favourite(const QString statusId)
    {
        const QString endpoint = QString("/api/v1/statuses/%1/favourite").arg(statusId);
        
        auto *reply = this->POST(endpoint, QVariantMap());
        return new APIFutureResource<Status>(reply);
    }
    
    APIFutureResource<Status> *StatusesAPI::unfavourite(const QString statusId)
    {
        const QString endpoint = QString("/api/v1/statuses/%1/unfavourite").arg(statusId);
    
        auto *reply = this->POST(endpoint, QVariantMap());
        return new APIFutureResource<Status>(reply);
    }
    
}