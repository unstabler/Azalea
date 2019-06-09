#include "mastodon.hpp"

using namespace serialization;
using namespace deserialization;

template<> void fromJSON(v1::Account *destination, QJsonObject source)
{
    destination->id = source["id"].toString();
    destination->displayName = source["display_name"].toString();
    destination->acct = source["acct"].toString();
    destination->username = source["username"].toString();
    destination->locked = BOOL(source["locked"]);
    
    destination->avatar = source["avatar"].toString();
    destination->avatarStatic = source["avatar_static"].toString();
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

    destination->createdAt = QDateTime::fromString(source["created_at"].toString(), Qt::ISODate);
    destination->content = source["content"].toString();
}

template<>
void toJSON(v1::in::PostStatusArgs *source, QJsonObject &destination)
{
    SET_OPTIONAL(destination["status"], source->status);
    SET_OPTIONAL(destination["in_reply_to_id"], source->inReplyToId);
    SET_OPTIONAL(destination["mediaIds"], source->mediaIds);
    SET_OPTIONAL(destination["sensitive"], source->sensitive);
    SET_OPTIONAL(destination["spoiler_text"], source->spoilerText);
    SET_OPTIONAL(destination["visibility"], source->visibility);
}

template<>
void toJSON(v1::in::TimelinesAPIArgs *source, QJsonObject &destination)
{
    SET_OPTIONAL(destination["max_id"], source->maxId);
    SET_OPTIONAL(destination["since_id"], source->sinceId);
    SET_OPTIONAL(destination["min_id"], source->minId);
    SET_OPTIONAL(destination["limit"], source->limit);
}
