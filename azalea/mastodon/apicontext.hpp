#pragma once

#include <QObject>
#include <QNetworkAccessManager>

class APIContext : public QObject
{
        Q_OBJECT
    public:
        explicit APIContext(QObject *parent = nullptr);

        bool forceUseHttp() const;
        QString host() const;
        void setHost(const QString host);
        QString token() const;
        void setToken(const QString token);

        QNetworkAccessManager* httpClient() const;

    private:
        QNetworkAccessManager* _httpClient;

        bool _forceUseHttp;
        QString _host;
        QString _token;
};
