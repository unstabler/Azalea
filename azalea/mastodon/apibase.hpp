#pragma once

#include <google/protobuf/util/json_util.h>

#include <QScopedPointer>
#include <QMap>
#include <QObject>
#include <QNetworkReply>

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
            google::protobuf::util::JsonParseOptions parseOptions;
            parseOptions.ignore_unknown_fields = true;  // FIXME: 가능하면 재사용 

            T* instance = new T();

            auto status = google::protobuf::util::JsonStringToMessage(this->body().toStdString(), instance, parseOptions);

            if (!status.ok()) {
                // FIXME: 예외 처리
                std::cerr << status.ToString() << std::endl;
            }
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
        QNetworkReply* DELETE(const QString endpoint,
                              const ParamMap& form);

        // TODO: MULTIPART 대응
    private:
        APIContext* _context;
};

