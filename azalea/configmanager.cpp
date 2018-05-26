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

bool ConfigManager::isDataDirectoryExists() const
{
    return QDir(dataDirectory()).exists();
}

void ConfigManager::tryCreateDataDirectory() const
{
    QDir().mkpath(dataDirectory());
}
