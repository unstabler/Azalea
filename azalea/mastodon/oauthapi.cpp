#include <QUrl>
#include <QUrlQuery>

#include "v1/appsapi.hpp"
#include "oauthapi.hpp"

OAuthAPI::OAuthAPI(APIContext *context) :
    APIBase(context)
{

}

QUrl OAuthAPI::getAuthorizeUrl(const QString clientId, const QString scope)
{
    auto url = this->buildUrl("/oauth/authorize");
    QUrlQuery query;
    query.addQueryItem("response_type", "code");
    query.addQueryItem("client_id", clientId);
    query.addQueryItem("redirect_uri", v1::AppsAPI::NO_REDIRECT_URIS);
    query.addQueryItem("scope", scope);

    url.setQuery(query);
    return url;
}

APIFutureResource<oauth2::OAuthToken>* OAuthAPI::tokenByAuthorizationCode(const QString clientId,
                                                                 const QString clientSecret,
                                                                 const QString code)
{
    static const QString ENDPOINT = "/oauth/token";
    ParamMap params;
    params["grant_type"] = "authorization_code";
    params["redirect_uri"] = v1::AppsAPI::NO_REDIRECT_URIS;
    params["client_id"] = clientId;
    params["client_secret"] = clientSecret;
    params["code"] = code;

    QNetworkReply *reply = this->POST(ENDPOINT, params);
    return new APIFutureResource<oauth2::OAuthToken>(reply);
}
