#include "appsapi.hpp"

#include <QNetworkReply>
#include <QUrlQuery>

namespace v1
{
    const QString AppsAPI::NO_REDIRECT_URIS = "urn:ietf:wg:oauth:2.0:oob";

    AppsAPI::AppsAPI(APIContext *context) :
        APIBase(context)
    {

    }

    APIFutureResource<Application>* AppsAPI::post(const QString clientName,
                                                 const QString redirectUris,
                                                 const QString scopes,
                                                 const QString website)
    {
        static const QString ENDPOINT = "/api/v1/apps";
        ParamMap params;
        params["client_name"]   = clientName;
        params["redirect_uris"] = redirectUris;
        params["scopes"]  = scopes;
        params["website"] = website;

        QNetworkReply *reply = this->POST(ENDPOINT, params);

        return new APIFutureResource<Application>(reply);
    }
}
