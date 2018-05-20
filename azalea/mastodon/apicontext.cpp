#include "apicontext.hpp"

APIContext::APIContext(QObject *parent) :
    QObject(parent),
    _httpClient(new QNetworkAccessManager(this)),
    _forceUseHttp(false)
{

}

bool APIContext::forceUseHttp() const
{
    return _forceUseHttp;
}

QString APIContext::host() const
{
    return _host;
}

void APIContext::setHost(const QString host)
{
    _host = host;
}

QString APIContext::token() const
{
    return _token;
}

void APIContext::setToken(const QString token)
{
    _token = token;
}

QNetworkAccessManager* APIContext::httpClient() const
{
    return _httpClient;
}
