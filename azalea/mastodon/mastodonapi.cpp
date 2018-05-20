#include "mastodonapi.hpp"

MastodonAPI::MastodonAPI(APIContext *context) :
    QObject(context),
    _context(context)
{

}

APIContext* MastodonAPI::context() const
{
    return _context;
}
