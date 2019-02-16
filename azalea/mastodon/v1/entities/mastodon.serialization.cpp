#include "mastodon.hpp"

using namespace deserialization;

template<> void fromJSON(v1::Account *destination, QJsonObject source)
{

}

template<> void fromJSON(v1::Status *destination, QJsonObject source)
{
    destination->id = source["id"].toString();
    destination->uri = source["uri"].toString();
    destination->url = source["url"].toString();

    destination->account = OBJECT<v1::Account>(source["account"]);

    destination->inReplyToId = source["in_reply_to_id"].toString();
    destination->inReplyToAccountId = source["in_reply_to_account_id"].toString();

    // TODO: null-check
    /*
    if (!source["reblog"].isUndefined()) {
        destination->reblog = new v1::Status;
        fromJSON(destination->reblog, source["reblog"].toObject());
    }
    */

    // TODO: FORMAT 판정 
    destination->createdAt = QDateTime::fromString(source["created_at"].toString());


    destination->content = source["content"].toString();
}