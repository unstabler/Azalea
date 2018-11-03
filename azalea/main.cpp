#include <QApplication>

#include "azaleaapplication.hpp"
#include "singleton.hpp"

#include "google/protobuf/stubs/common.h"

int main(int argc, char *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    AzaleaApplication* app = new AzaleaApplication(argc, argv);
    const auto returnValue = app->exec();

    google::protobuf::ShutdownProtobufLibrary();

    SingletonFinalizer::finalize();
    return returnValue;
}

