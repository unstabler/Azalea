#pragma once

#include <QObject>
#include <QtWebSockets/QtWebSockets>

#include "mastodon/apicontext.hpp"

class StreamingClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(StreamingClient)
    
public:
    explicit StreamingClient(APIContext &context, QString streamType);
    
    void open();
    void close();
    bool isConnectionOpen();
    
signals:
    void connected();
    void disconnected();
    void streamEvent(QString eventType, QJsonObject payload);
    
private slots:
    void messageReceived(QString message);

private:
    APIContext &_context;
    QString _streamType;
    
    QWebSocket _wsClient;
    
    QUrl createEndpointUrl();
};

