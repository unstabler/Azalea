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

QByteArray APIBase::serializeParamMap(const ParamMap& paramMap)
{
    QUrlQuery query;
    for (auto key: paramMap.keys())
    {
        query.addQueryItem(key, paramMap[key]);
    }

    qDebug() << query.toString(QUrl::FullyEncoded);

    return query.toString(QUrl::FullyEncoded).toUtf8();
}

QNetworkReply* APIBase::POST(const QString endpoint, const ParamMap& form)
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
    connect(_reply, SIGNAL(finished()), this, SLOT(requestFinished()));
    connect(_reply, SIGNAL(error()),    this, SLOT(requestError()));
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
    qDebug() << "REQUEST FINISHED";
    _body = tr(_reply->readAll());

    emit resolved();
}

void APIFutureResponse::requestError()
{
    qDebug() << "REQUEST FAILED";
    int responseCode = _reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute
    ).toInt();

    emit rejected(responseCode, _reply->readAll());
}
