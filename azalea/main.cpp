#include "ui/mainwindow.hpp"
#include <QApplication>

#include "singleton.hpp"

static const QString applicationName = "Azalea";

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName(applicationName);
#ifndef AZALEA_VERSION
    application.setApplicationVersion("unstable");
#elif
    application.setApplicationVersion(AZALEA_VERSION);
#endif
    MainWindow window;
    window.show();
    const auto returnValue = application.exec();

    SingletonFinalizer::finalize();
    return returnValue;
}
