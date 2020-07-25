cmake_minimum_required(VERSION 3.10)

include(${PROJECT_SOURCE_DIR}/cmake/googletest_build.cmake)

add_executable(test2 test2.cpp)
target_link_libraries(test2 torima gtest_main)

add_executable(test2-1 test2-1.cpp)
target_link_libraries(test2-1 torima gtest_main)

include_directories(${PROJECT_SOURCE_DIR}/src)

include(GoogleTest)
gtest_add_tests(TARGET test2)
gtest_add_tests(TARGET test2-1)
