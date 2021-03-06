#include "timelinesapi.hpp"

namespace v1
{
    
    TimelinesAPI::TimelinesAPI(APIContext *context) :
            APIBase(context)
    {
    
    }
    
    APIFutureResource<ResourceList<Status>> *TimelinesAPI::home(in::TimelinesAPIArgs &args)
    {
        static const QString ENDPOINT = "/api/v1/timelines/home";
        QVariantMap argsMap = toJSON(&args).toVariantMap();
        
        return this->_timeline(ENDPOINT, argsMap);
    }
    
    APIFutureResource<ResourceList<Status>> *TimelinesAPI::local(in::TimelinesAPIArgs &args)
    {
        return this->_public(args, true);
    }
    
    APIFutureResource<ResourceList<Status>> *TimelinesAPI::federated(in::TimelinesAPIArgs &args)
    {
        return this->_public(args, false);
    }
    
    APIFutureResource<ResourceList<Status>> *TimelinesAPI::_public(in::TimelinesAPIArgs &args, const bool local)
    {
        static const QString ENDPOINT = "/api/v1/timelines/public";
        QVariantMap argsMap = toJSON(&args).toVariantMap();
        argsMap["local"] = local;
    
        return this->_timeline(ENDPOINT, argsMap);
    }
    
    APIFutureResource<ResourceList<Status>> *TimelinesAPI::_timeline(const QString endpoint, QVariantMap argsMap)
    {
        QNetworkReply *reply = this->GET(endpoint, argsMap);
        return new APIFutureResource<ResourceList<Status>>(reply);
    }
    
}