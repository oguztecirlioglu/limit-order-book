cmake_minimum_required(VERSION 3.30)

project(CLOB
        VERSION 1.0
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(clob ${SOURCES})