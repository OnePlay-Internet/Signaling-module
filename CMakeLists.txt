# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

if(MSVC)
    set (VCPKG_HOME "/vcpkg") #IMPORTANT: change this to where your vcpkg exists.
    set (CMAKE_TOOLCHAIN_FILE "${VCPKG_HOME}/scripts/buildsystems/vcpkg.cmake")
endif()

project(signaling-module)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(rtsp)
