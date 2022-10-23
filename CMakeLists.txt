# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (VCPKG_HOME "${CMAKE_CURRENT_LIST_DIR}/vcpkg") #IMPORTANT: change this to where your vcpkg exists.
set (CMAKE_TOOLCHAIN_FILE "${VCPKG_HOME}/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 17)

project(signaling-module)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(rtsp)
