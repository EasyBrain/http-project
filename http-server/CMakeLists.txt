cmake_minimum_required(VERSION 3.16)
project(http_server)

set(CMAKE_CXX_STANDARD 17)

include_directories(modules/ext/filesystem/include)

add_subdirectory(modules/ext/filesystem)

find_package(Poco REQUIRED COMPONENTS Util Net)

add_executable(http_server main.cpp RequestHandlerFactory.cpp RequestHandlerFactory.hpp
        RequestHandlers.cpp RequestHandlers.hpp ServerApp.cpp ServerApp.h)
target_link_libraries(http_server ext-filesystem Poco::Util Poco::Net)