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

template<>
void fromJSON(v1::Attachment *destination, QJsonObject source)
{
    destination->id = source["id"].toString();
    destination->type = source["type"].toString();
    destination->url = source["url"].toString();
    destination->remoteUrl = source["remote_url"].toString();
    destination->previewUrl = source["preview_url"].toString();
    destination->textUrl = source["text_url"].toString();
    // TODO: meta
    destination->description = source["description"].toString();
}

template<>
void fromJSON(v1::Emoji *destination, QJsonObject source)
{
    destination->shortcode = source["shortcode"].toString();
    destination->staticUrl = QUrl(source["static_url"].toString());
    destination->url = QUrl(source["url"].toString());
    destination->visibleInPicker = BOOL(source["visible_in_picker"]);
}

template<> void fromJSON(v1::Status *destination, QJsonObject source)
{
    destination->id = source["id"].toString();
    destination->uri = source["uri"].toString();
    destination->url = source["url"].toString();

    destination->account = OBJECT_SHAREDPTR<v1::Account>(source["account"]);
    destination->mediaAttachements = ARRAY_SHAREDPTR<v1::Attachment>(source["media_attachments"]);

    destination->inReplyToId = source["in_reply_to_id"].toString();
    destination->inReplyToAccountId = source["in_reply_to_account_id"].toString();
    
    destination->reblogged  = BOOL(source["reblogged"]);
    destination->favourited = BOOL(source["favourited"]);
    
    destination->reblog = OBJECT_SHAREDPTR<v1::Status>(source["reblog"]);
    destination->content = source["content"].toString();
    
    destination->createdAt = QDateTime::fromString(source["created_at"].toString(), Qt::ISODate);
    destination->emojis = ARRAY_SHAREDPTR<v1::Emoji>(source["emojis"]);
}

template<>
void fromJSON(v1::Notification *destination, QJsonObject source)
{
    destination->id = source["id"].toString();
    destination->type = source["type"].toString();
    destination->createdAt = QDateTime::fromString(source["created_at"].toString(), Qt::ISODate);
    destination->account = OBJECT_SHAREDPTR<v1::Account>(source["account"]);
    destination->status = OBJECT_SHAREDPTR<v1::Status>(source["status"]);
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

template<>
void toJSON(v1::in::NotificationListArgs *source, QJsonObject &destination)
{
    SET_OPTIONAL(destination["max_id"], source->maxId);
    SET_OPTIONAL(destination["since_id"], source->sinceId);
    SET_OPTIONAL(destination["min_id"], source->minId);
    SET_OPTIONAL(destination["limit"], source->limit);
    SET_OPTIONAL(destination["exclude_types"], source->excludeTypes);
    SET_OPTIONAL(destination["account_id"], source->accountId);
}
