#pragma once

#include <QScopedPointer>
#include <QMap>
#include <QObject>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include "serialization.hpp"
#include "apicontext.hpp"

using ParamMap = QMap<QString, QString>;

class APIFutureResponse : public QObject
{
        Q_OBJECT
        Q_DISABLE_COPY(APIFutureResponse)

    public:
        APIFutureResponse(QNetworkReply* reply);
        QString body() const;


    protected:
        QNetworkReply* reply() const;

    signals:
        void resolved();
        void rejected(int code, QString content);

    private slots:
        void requestFinished();
        void requestError(QNetworkReply::NetworkError error);

    private:
        QNetworkReply* _reply;
        QString _body;
};

/**
 * FIXME: 왜 이거 함수가 코드로 분리되어 있으면 undefined reference 오류가 나는지 모르겠어요 X(
 * TODO: cpp로 옮기기
 */
template<class T>
class APIFutureResource : public APIFutureResponse
{
    public:
        APIFutureResource(QNetworkReply* reply) :
            APIFutureResponse(reply)
        {

        }

        QSharedPointer<T> tryDeserialize()
        {
            T* instance = new T();
            fromJSON(
                instance,
                // object가 아닌 경우는?!
                QJsonDocument::fromJson(this->body().toUtf8()).object()
            );

            return QSharedPointer<T>(instance);
        }
};

template<class T>
class APIFutureResourceList : public APIFutureResource<QList<T>> 
{
    public:
        QSharedPointer<QList<T>> tryDeserialize()
        {
            return QSharedPointer<T>(nullptr);
        }
};

class APIBase : public QObject
{
        Q_OBJECT
    public:
        explicit APIBase(APIContext *context);

    protected:
        APIContext* context() const;
        QNetworkAccessManager* httpClient() const;

        QUrl buildUrl(const QString endpoint);
        QNetworkRequest buildRequest(const QString endpoint);
        QNetworkRequest buildRequest(const QUrl url);

        QByteArray serializeParamMap(const ParamMap& paramMap);

        QNetworkReply* GET(const QString endpoint,
                           const ParamMap& params);
        QNetworkReply* POST(const QString endpoint,
                            const ParamMap& form);
        QNetworkReply* DELETE(const QString endpoint,
                              const ParamMap& form);

        // TODO: MULTIPART 대응
    private:
        APIContext* _context;
};

