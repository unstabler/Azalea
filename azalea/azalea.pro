#-------------------------------------------------
#
# Project created by QtCreator 2018-05-19T15:20:13
#
#-------------------------------------------------

QT     += core gui network
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Azalea
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        ui/mainwindow.cpp \
        mastodon/apicontext.cpp \
        mastodon/mastodonapi.cpp \
        mastodon/apibase.cpp \
        mastodon/v1/appsapi.cpp \
        ui/postarea.cpp \
    singleton.cpp

HEADERS += \
        serialization.hpp \
        ui/mainwindow.hpp \
        mastodon/apicontext.hpp \
        mastodon/mastodonapi.hpp \
        mastodon/apibase.hpp \
        mastodon/v1/entities/__serialization_helper.hpp \
        mastodon/v1/entities/application.hpp \
        mastodon/v1/appsapi.hpp \
        mastodon/v1.hpp \
        ui/postarea.hpp \
    singleton.hpp

FORMS += \
        ui/mainwindow.ui \
        ui/postarea.ui


INSTALLS += target 
target.path = $$PREFIX/bin
