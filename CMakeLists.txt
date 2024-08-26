cmake_minimum_required(VERSION 3.30)

project(CLOB
        VERSION 1.0
        LANGUAGES CXX)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(clob ${SOURCES})