#include "mastodonapi.hpp"

MastodonAPI::MastodonAPI(APIContext *context) :
    QObject(context),
    _context(context),
    _apps(new v1::AppsAPI(context))
{

}

APIContext* MastodonAPI::context() const
{
    return _context;
}

v1::AppsAPI* MastodonAPI::apps() const
{
    return _apps;
}
