#include "mastodonapi.hpp"

MastodonAPI::MastodonAPI(APIContext *context) :
    QObject(context),
    _context(context),
    _oauth(new OAuthAPI(context)),
    _apps(new v1::AppsAPI(context)),
    _timelines(new v1::TimelinesAPI(context)),
    _statuses(new v1::StatusesAPI(context)),
    _notifications(new v1::NotificationsAPI(context))
{

}

APIContext *MastodonAPI::context() const
{
    return _context;
}

OAuthAPI *MastodonAPI::oauth() const
{
    return _oauth;
}

v1::AppsAPI *MastodonAPI::apps() const
{
    return _apps;
}

v1::TimelinesAPI *MastodonAPI::timelines() const
{
    return _timelines;
}

v1::StatusesAPI *MastodonAPI::statuses() const
{
    return _statuses;
}

v1::NotificationsAPI *MastodonAPI::notifications() const
{
    return _notifications;
}
