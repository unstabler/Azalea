//
// Created by cheesekun on 2019/10/06.
//

#include "instanceapi.hpp"

namespace v1
{
    InstanceAPI::InstanceAPI(APIContext *context) :
        APIBase(context)
    {
    
    }
    
    APIFutureResource<ResourceList<Emoji>> *InstanceAPI::getCustomEmojiList()
    {
        static const QString ENDPOINT = "/api/v1/custom_emojis";
        
        QNetworkReply *reply = this->GET(ENDPOINT, QVariantMap());
        return new APIFutureResource<ResourceList<Emoji>>(reply);
    }
}