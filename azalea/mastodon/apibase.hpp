#pragma once

#include <sstream>

#include <QScopedPointer>
#include <QMap>
#include <QObject>
#include <QNetworkReply>

#include <cereal/archives/json.hpp>

#include "apicontext.hpp"
#include "serialization.hpp"

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
        void requestError();

    private:
        QNetworkReply* _reply;
        QString _body;
};


/**
 * FIXME: 왜 이거 함수가 코드로 분리되어 있으면 undefined reference 오류가 나는지 모르겠어요 X(
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
            std::stringstream stream(body().toStdString());
            T* instance = new T();
            cereal::JSONInputArchive archive(stream);

            serialize(archive, *instance);
            return QSharedPointer<T>(instance);
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

        // TODO: MULTIPART 대응
    private:
        APIContext* _context;
};

