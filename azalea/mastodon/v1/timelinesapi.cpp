#include "timelinesapi.hpp"

namespace v1
{
    
    TimelinesAPI::TimelinesAPI(APIContext *context) :
            APIBase(context)
    {
    
    }
    
    APIFutureResource<QList<Status*>> *TimelinesAPI::home(in::TimelinesAPIArgs &args)
    {
        static const QString ENDPOINT = "/api/v1/timelines/home";
        QVariantMap params; // FIXME
        
        return this->_timeline(ENDPOINT, params);
    }
    
    APIFutureResource<QList<Status*>> *TimelinesAPI::local(in::TimelinesAPIArgs &args)
    {
        return this->_public(args, true);
    }
    
    APIFutureResource<QList<Status*>> *TimelinesAPI::federated(in::TimelinesAPIArgs &args)
    {
        return this->_public(args, false);
    }
    
    APIFutureResource<QList<Status*>> *TimelinesAPI::_public(in::TimelinesAPIArgs &args, const bool local)
    {
        static const QString ENDPOINT = "/api/v1/timelines/public";
        QVariantMap params; // FIXME
        params["local"] = local;
    
        return this->_timeline(ENDPOINT, params);
    }
    
    APIFutureResource<QList<Status*>> *TimelinesAPI::_timeline(const QString endpoint, QVariantMap params)
    {
        QNetworkReply *reply = this->GET(endpoint, params);
        return new APIFutureResource<QList<Status*>>(reply);
    }
    
}