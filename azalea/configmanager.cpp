#include "configmanager.hpp"

#include <algorithm>

#include <QtGlobal>
#include <QDir>
#include <QStandardPaths>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "serialization.hpp"
#include "singleton.hpp"

using namespace serialization;
using namespace deserialization;

ConfigManager::ConfigManager(QObject *parent) :
    QObject(parent),
    _configPath(dataDirectory() + "/config.json"),
    _credentials(new QList<Credential*>),
    _registeredInstances(new QList<InstanceInfo*>)
{
    if (!isDataDirectoryExists()) {
        tryCreateDataDirectory();
    }
}

QString ConfigManager::dataDirectory() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

void ConfigManager::save()
{
    QJsonObject rawConfigRoot;
    rawConfigRoot.insert("credentials", TO_JSONARRAY(
            _credentials->begin(),
            _credentials->end()
    ));
    rawConfigRoot.insert("instances", TO_JSONARRAY(
            _registeredInstances->begin(),
            _registeredInstances->end()
    ));
    
    QJsonDocument rawConfigDocument(rawConfigRoot);
    QFile rawConfigFile(_configPath);
    
    if (!rawConfigFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open configuration file: %s", qUtf8Printable(_configPath));
        return;
    }
    
    rawConfigFile.write(rawConfigDocument.toJson());
    rawConfigFile.close();
}

void ConfigManager::load()
{
    QFile rawConfigFile(_configPath);
    
    if (!rawConfigFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open configuration file: %s", qUtf8Printable(_configPath));
        save();
        return;
    }
    
    QJsonDocument rawConfigDocument = QJsonDocument::fromJson(rawConfigFile.readAll());
    QJsonObject rawConfigRoot = rawConfigDocument.object();
    
    rawConfigFile.close();
    
    if (rawConfigRoot.contains("credentials")) {
        _credentials = ARRAY<Credential>(rawConfigRoot["credentials"]);
    }
    if (rawConfigRoot.contains("instances")) {
        _registeredInstances = ARRAY<InstanceInfo>(rawConfigRoot["instances"]);
    }
}

bool ConfigManager::isDataDirectoryExists() const
{
    return QDir(dataDirectory()).exists();
}

void ConfigManager::tryCreateDataDirectory() const
{
    QDir().mkpath(dataDirectory());
}

QList<InstanceInfo *> *ConfigManager::registeredInstances() const
{
    return _registeredInstances;
}

void ConfigManager::setRegisteredInstances(QList<InstanceInfo *> *registeredInstances)
{
    _registeredInstances = registeredInstances;
}

QList<Credential *> *ConfigManager::credentials() const
{
    return _credentials;
}

void ConfigManager::setCredentials(QList<Credential *> *credentials)
{
    _credentials = credentials;
}

InstanceInfo::InstanceInfo()
{

}

InstanceInfo::InstanceInfo(const QString &instanceName, oauth2::OAuthApplication *application) :
        _instanceName(instanceName),
        _application(application)
{

}

const QString &InstanceInfo::instanceName() const {
    return _instanceName;
}

void InstanceInfo::setInstanceName(const QString &instanceName) {
    InstanceInfo::_instanceName = instanceName;
}

oauth2::OAuthApplication *InstanceInfo::application() const {
    return _application;
}

void InstanceInfo::setApplication(oauth2::OAuthApplication *application) {
    InstanceInfo::_application = application;
}

Credential::Credential()
{

}

Credential::Credential(const QString &instanceName, const QString &username, oauth2::OAuthToken *token) :
        _instanceName(instanceName),
        _username(username),
        _token(token)
{

}

const QString &Credential::instanceName() const
{
    return _instanceName;
}

void Credential::setInstanceName(const QString &instanceName)
{
    _instanceName = instanceName;
}

const QString &Credential::username() const
{
    return _username;
}

void Credential::setUsername(const QString &username)
{
    _username = username;
}

oauth2::OAuthToken *Credential::token() const
{
    return _token;
}

void Credential::setToken(oauth2::OAuthToken *token)
{
    _token = token;
}

template<> void toJSON(InstanceInfo *source, QJsonObject &destination)
{
    destination["instance_name"] = source->instanceName();
    destination["application"] = toJSON(source->application());
}

template<> void fromJSON(InstanceInfo *destination, QJsonObject source)
{
    destination->setInstanceName(STRING(source["instance_name"]));
    destination->setApplication(OBJECT<oauth2::OAuthApplication>(source["application"]));
}


template<> void toJSON(Credential *source, QJsonObject &destination)
{
    destination["instance_name"] = source->instanceName();
    destination["username"] = source->username();
    destination["token"] = toJSON(source->token());
}

template<> void fromJSON(Credential *destination, QJsonObject source)
{
    destination->setInstanceName(STRING(source["instance_name"]));
    destination->setUsername(STRING(source["username"]));
    destination->setToken(OBJECT<oauth2::OAuthToken>(source["token"]));
}
