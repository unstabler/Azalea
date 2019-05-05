#include "mastodon.hpp"

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
