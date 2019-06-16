#include "azaleaapplication.hpp"

const QString AzaleaApplication::APPLICATION_NAME = "Azalea";

#ifndef AZALEA_VERSION
    #ifdef AZALEA_GIT_COMMIT_HASH
        const QString AzaleaApplication::APPLICATION_VERSION = QString("version #%1").arg(
                AZALEA_GIT_COMMIT_HASH
        );
    #elif
        const QString AzaleaApplication::APPLICATION_VERSION = "unknown-unstable";
    #endif
#elif
const QString AzaleaApplication::APPLICATION_VERSION = AZALEA_VERSION;
#endif

AzaleaApplication::AzaleaApplication(int &argc, char **argv) :
    QApplication(argc, argv),
    mainWindow(nullptr),
    _translator(this)
{
    setApplicationName(APPLICATION_NAME);
    setApplicationVersion(APPLICATION_VERSION);
    
    
    QString locale = QLocale::system().name().toLower();
    this->loadTranslation(locale);
}

void AzaleaApplication::loadTranslation(QString locale, bool fallbackOnFailure)
{
    if (_translator.load(QString("%1.qm").arg(locale), ":/translations/")) {
        this->installTranslator(&_translator);
    } else {
        if (fallbackOnFailure) {
            qDebug() << QString("translations for %1 is not available yet; fallback to ko_KR").arg(locale);
            this->loadTranslation("ko_kr", false);
        }
    }
}

int AzaleaApplication::exec()
{
    mainWindow = new MainWindow;
    mainWindow->show();
    mainWindow->setWindowTitle(QString("%1 [%2]").arg(APPLICATION_NAME, APPLICATION_VERSION));
    return QApplication::exec();
}


