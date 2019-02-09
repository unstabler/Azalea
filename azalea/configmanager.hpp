#pragma once

#include <QObject>

#include "mastodon/v1/entities/oauth2.hpp"

class InstanceInfo {
public:
    InstanceInfo();
    InstanceInfo(const QString &instanceName, oauth2::OAuthApplication *application);
    
    const QString &instanceName() const;
    void setInstanceName(const QString &instanceName);

    oauth2::OAuthApplication *application() const;
    void setApplication(oauth2::OAuthApplication *application);

private:
    QString _instanceName;
    oauth2::OAuthApplication *_application;
};

class Credential
{
public:
    Credential();
    Credential(const QString &instanceName, const QString &username, oauth2::OAuthToken *token);
    
    const QString &instanceName() const;
    void setInstanceName(const QString &instanceName);
    
    const QString &username() const;
    void setUsername(const QString &username);
    
    oauth2::OAuthToken *token() const;
    void setToken(oauth2::OAuthToken *token);

private:
    QString _instanceName;
    QString _username;
    oauth2::OAuthToken *_token;
};

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);

    /** 데이터 디렉토리를 반환합니다 */
    QString dataDirectory() const;

    void save();
    void load();
    
    QList<InstanceInfo *> *registeredInstances() const;
    
    void setRegisteredInstances(QList<InstanceInfo *> *registeredInstances);
    
    QList<Credential *> *credentials() const;
    
    void setCredentials(QList<Credential *> *credentials);

signals:

public slots:

protected:
    /** 데이터 디렉토리가 파일 시스템 상에 존재하는지 확인합니다 */
    bool isDataDirectoryExists() const;
    /** 데이터 디렉토리를 생성합니다 */
    void tryCreateDataDirectory() const;

private:
    QString _configPath;
    QList<InstanceInfo*> *_registeredInstances;
    QList<Credential*> *_credentials;
};


template<> void toJSON(InstanceInfo *source, QJsonObject &destination);
template<> void fromJSON(InstanceInfo *destination, QJsonObject source);

template<> void toJSON(Credential *source, QJsonObject &destination);
template<> void fromJSON(Credential *destination, QJsonObject source);
