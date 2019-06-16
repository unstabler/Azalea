//
// Created by cheesekun on 19/06/16.
//

#include "notificationsapi.hpp"

namespace v1
{
    NotificationsAPI::NotificationsAPI(APIContext *context) :
            APIBase(context)
    {
    
    }
    
    APIFutureResource<ResourceList<Notification>> *NotificationsAPI::list(v1::in::NotificationListArgs &args)
    {
        static const QString ENDPOINT = "/api/v1/notifications";
        QVariantMap argsMap = toJSON(&args).toVariantMap();
        
        QNetworkReply *reply = this->GET(ENDPOINT, argsMap);
        return new APIFutureResource<ResourceList<Notification>>(reply);
    }
}