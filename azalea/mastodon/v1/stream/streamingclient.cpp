#include "streamingclient.hpp"

#include <QWebSocket>

StreamingClient::StreamingClient(APIContext &context, QString streamType) :
    QObject(nullptr),
    
    _context(context),
    _streamType(streamType),
    _wsClient()
{
    connect(&_wsClient, &QWebSocket::connected, [=]() {
        qDebug() << "connected to stream " << streamType;
        emit connected();
    });
    connect(&_wsClient, &QWebSocket::disconnected, [=]() {
        qDebug() << "disconnected from stream " << streamType;
        emit disconnected();
    });
    connect(&_wsClient,
            QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            [=](QAbstractSocket::SocketError error) {
            qDebug() << error;
            qDebug() << _wsClient.errorString();
    });
    
    connect(&_wsClient, &QWebSocket::textMessageReceived, this, &StreamingClient::messageReceived);
}

QUrl StreamingClient::createEndpointUrl()
{
    QUrlQuery query;
    query.addQueryItem("stream", _streamType);
    query.addQueryItem("access_token", _context.token());
    
    QUrl url;
    url.setScheme(
        _context.forceUseHttp() ? "ws" : "wss"
    );

    url.setHost(_context.host());
    url.setPath("/api/v1/streaming");
    url.setQuery(query);
    
    qDebug() << url;

    return url;
}

void StreamingClient::open()
{
    QUrl endpoint = createEndpointUrl();
    _wsClient.open(endpoint);
    qDebug() << "connecting to userstream";
}

void StreamingClient::close()
{
    _wsClient.close();
}



void StreamingClient::messageReceived(QString message)
{
    auto event = QJsonDocument::fromJson(message.toUtf8()).object();
    auto payload = QJsonDocument::fromJson(event["payload"].toString().toUtf8()).object();
    emit streamEvent(event["event"].toString(), payload);
}
