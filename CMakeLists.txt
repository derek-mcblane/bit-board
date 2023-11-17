cmake_minimum_required(VERSION 3.15.0)

project(BitBoard VERSION 0.1.0)

add_subdirectory(external)
add_subdirectory(source)

option(${PROJECT_NAME}_ENABLE_TESTING "Enable project tests" OFF)
if (${${PROJECT_NAME}_ENABLE_TESTING})
    enable_testing()
    add_subdirectory(tests)
endif()
