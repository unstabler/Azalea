#include <QApplication>

#include "azaleaapplication.hpp"
#include "singleton.hpp"

int main(int argc, char *argv[])
{
    AzaleaApplication app(argc, argv);
    MainWindow window;
    window.show();
    const auto returnValue = app.exec();

    SingletonFinalizer::finalize();
    return returnValue;
}
