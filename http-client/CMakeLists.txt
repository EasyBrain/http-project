cmake_minimum_required(VERSION 3.16)
project(http_client)

set(CMAKE_CXX_STANDARD 17)

find_package(Poco REQUIRED COMPONENTS Foundation Util Net)

include_directories(modules/ext/filesystem/include)
add_subdirectory(modules/ext/filesystem)

add_executable(http_client main.cpp Client.cpp Client.h)
target_link_libraries(http_client ext-filesystem Poco::Foundation Poco::Util Poco::Net)