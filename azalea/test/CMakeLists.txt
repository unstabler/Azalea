# Find the QtWidgets library

find_package(Qt5Test CONFIG REQUIRED)

include_directories(..)

add_executable(test_optional test_optional.cpp)
add_executable(
        test_statusformatter
        test_statusformatter.cpp
        ../ui/timeline/statusformatter.cpp
        ../mastodon/v1/entities/mastodon.serialization.cpp
)
target_link_libraries(test_optional Qt5::Core Qt5::Test)
target_link_libraries(test_statusformatter Qt5::Core Qt5::Test)
add_test(azalea_test test_optional test_statusformatter)


