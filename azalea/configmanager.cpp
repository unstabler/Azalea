#include "configmanager.hpp"

#include <QDir>
#include <QStandardPaths>

#include "singleton.hpp"

ConfigManager::ConfigManager(QObject *parent) :
    QObject(parent)
{
    if (!isDataDirectoryExists()) {
        tryCreateDataDirectory();
    }
}

QString ConfigManager::dataDirectory() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString ConfigManager::token() const
{
    return _token;
}

void ConfigManager::load()
{

}

QString ConfigManager::instanceName() const
{
    return _instanceName;
}

void ConfigManager::setToken(const QString &token)
{
    _token = token;
}

void ConfigManager::setInstanceName(const QString &instanceName)
{
    _instanceName = instanceName;
}

bool ConfigManager::isDataDirectoryExists() const
{
    return QDir(dataDirectory()).exists();
}

void ConfigManager::tryCreateDataDirectory() const
{
    QDir().mkpath(dataDirectory());
}
