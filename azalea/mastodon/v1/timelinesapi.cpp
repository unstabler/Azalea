#include "timelinesapi.hpp"

namespace v1
{
    
    TimelinesAPI::TimelinesAPI(APIContext *context) :
            APIBase(context)
    {
    
    }
    
    APIFutureResource<QList<Status>> *TimelinesAPI::home(in::TimelinesAPIArgs &args)
    {
        static const QString ENDPOINT = "/api/v1/timelines/home";
        QVariantMap params;
        
        QNetworkReply *reply = this->GET(ENDPOINT, params);
        return new APIFutureResource<QList<Status>>(reply);
    }
    
    
}