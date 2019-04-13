#include <QApplication>

#include "azaleaapplication.hpp"
#include "singleton.hpp"

int main(int argc, char *argv[])
{
    auto* app = new AzaleaApplication(argc, argv);
    const auto returnValue = app->exec();

    SingletonFinalizer::finalize();
    return returnValue;
}

