#include "apibase.hpp"

#include <QEventLoop>
#include <QNetworkReply>
#include <QUrlQuery>

APIBase::APIBase(APIContext* context) :
    QObject(context),
    _context(context)
{
}

APIContext* APIBase::context() const
{
    return _context;
}

QNetworkAccessManager* APIBase::httpClient() const
{
    return context()->httpClient();
}


QUrl APIBase::buildUrl(const QString endpoint)
{
    QUrl url;
    url.setScheme(
        context()->forceUseHttp() ? "http" : "https"
    );

    url.setHost(context()->host());
    url.setPath(endpoint);

    return url;
}

QNetworkRequest APIBase::buildRequest(const QString endpoint)
{
    return buildRequest(buildUrl(endpoint));
}

QNetworkRequest APIBase::buildRequest(const QUrl url)
{
    auto token = context()->token();
    QNetworkRequest request;
    request.setUrl(url);
    if (!token.isEmpty())
    {
        request.setRawHeader(
            "Authorization", QString("Bearer %1").arg(token).toUtf8()
        );
    }


    return request;
}

QByteArray APIBase::serializeParamMap(const QVariantMap& paramMap)
{
    QUrlQuery query;
    for (auto key: paramMap.keys())
    {
        auto strValue = paramMap[key].toString();
        if (!strValue.isEmpty()) {
            query.addQueryItem(
                    QUrl::toPercentEncoding(key),
                    QUrl::toPercentEncoding(paramMap[key].toString())
            );
        }
    }

    return query.toString().toUtf8();
}

QNetworkReply *APIBase::GET(const QString endpoint, const QVariantMap &params)
{
    auto request = buildRequest(endpoint);
    auto reply = httpClient()->get(request);
    
    return reply;
}

QNetworkReply *APIBase::POST(const QString endpoint, const QVariantMap& form)
{
    auto request = buildRequest(endpoint);
    auto data = serializeParamMap(form);
    auto reply = httpClient()->post(request, data);

    return reply;
}


APIFutureResponse::APIFutureResponse(QNetworkReply* reply) :
    QObject(reply),
    _reply(reply)
{
    connect(_reply, &QNetworkReply::finished, this, &APIFutureResponse::requestFinished);
    connect(_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &APIFutureResponse::requestError);
}

QString APIFutureResponse::body() const
{
    return _body;
}

QNetworkReply* APIFutureResponse::reply() const
{
    return _reply;
}

void APIFutureResponse::requestFinished()
{
    if (_reply->error() != QNetworkReply::NoError) {
        return;
    }
    _body = tr(_reply->readAll());

    emit resolved();
}

void APIFutureResponse::requestError(QNetworkReply::NetworkError error)
{
    int responseCode = _reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute
    ).toInt();

    if (responseCode == 0) {
        emit rejected(error, _reply->errorString());
    } else {
        emit rejected(responseCode, _reply->readAll());
    }
}
