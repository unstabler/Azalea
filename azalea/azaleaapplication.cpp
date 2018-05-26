#include "azaleaapplication.hpp"

QString AzaleaApplication::APPLICATION_NAME = "Azalea";

#ifndef AZALEA_VERSION
QString AzaleaApplication::APPLICATION_VERSION = AZALEA_VERSION;
#elif
QString AzaleaApplication::APPLICATION_VERSION = "unknown-unstable";
#endif

AzaleaApplication::AzaleaApplication(int argc, char* argv[]) :
    QApplication(argc, argv),
    mainWindow(this)
{
    setApplicationName(APPLICATION_NAME);
    setApplicationVersion(APPLICATION_VERSION);
}

int AzaleaApplication::exec()
{
    mainWindow->show();
    return QApplication::exec();
}
