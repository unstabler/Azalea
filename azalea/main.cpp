#include <QApplication>

#include "azaleaapplication.hpp"
#include "singleton.hpp"

int main(int argc, char *argv[])
{
    AzaleaApplication* app = new AzaleaApplication(argc, argv);
    const auto returnValue = app->exec();
    SingletonFinalizer::finalize();
    return returnValue;
}

