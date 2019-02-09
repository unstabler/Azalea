cmake_minimum_required(VERSION 3.0)
include(FindProtobuf)

project(Azalea)

set(CMAKE_CXX_STANDARD 14)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Quick CONFIG REQUIRED)
find_package(Qt5QuickWidgets)
find_package(Qt5QuickCompiler CONFIG REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

set(AZALEA_SRC
    main.cpp
    azaleaapplication.cpp
    configmanager.cpp
    singleton.cpp
        ui/timeline/StatusAdapter.cpp ui/timeline/StatusAdapter.hpp)

add_subdirectory(ui)
add_subdirectory(mastodon)

qtquick_compiler_add_resources(AZALEA_RES azalea.qrc)
# protobuf_generate_cpp(AZALEA_PROTO_SRC AZALEA_PROTO_HEADER ${AZALEA_PROTO})

# Tell CMake to create the helloworld executable
add_executable(azalea ${AZALEA_SRC} ${AZALEA_PROTO_SRC} ${AZALEA_PROTO_HEADER} ${AZALEA_RES})

# Use the Widgets module from Qt 5.
target_link_libraries(azalea Qt5::Widgets Qt5::Network Qt5::Quick Qt5::QuickWidgets ${PROTOBUF_LIBRARY})

# Install the executable
install(TARGETS azalea DESTINATION bin)
