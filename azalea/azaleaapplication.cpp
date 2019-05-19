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
    mainWindow(new MainWindow)
{
    setApplicationName(APPLICATION_NAME);
    setApplicationVersion(APPLICATION_VERSION);
}

int AzaleaApplication::exec()
{
    mainWindow->show();
    mainWindow->setWindowTitle(QString("%1 [%2]").arg(APPLICATION_NAME, APPLICATION_VERSION));
    return QApplication::exec();
}
