#include "azaleaapplication.hpp"

const QString AzaleaApplication::APPLICATION_NAME = "Azalea";

#ifndef AZALEA_VERSION
const QString AzaleaApplication::APPLICATION_VERSION = "unknown-unstable";
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
    return QApplication::exec();
}
