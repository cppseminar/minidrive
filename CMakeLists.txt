cmake_minimum_required(VERSION 3.22)

project(MiniDrive VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MINIDRIVE_BUILD_TESTS "Build MiniDrive tests" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Dependencies)

add_subdirectory(shared)
add_subdirectory(server)
add_subdirectory(client)

if(MINIDRIVE_BUILD_TESTS)
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)

