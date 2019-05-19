# Find the QtWidgets library

find_package(Qt5Test CONFIG REQUIRED)

add_executable(test_optional test_optional.cpp)
target_link_libraries(test_optional Qt5::Core Qt5::Test)
add_test(test_optional test_optional)

