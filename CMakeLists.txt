cmake_minimum_required(VERSION 3.10)
project(jinx)

set(CMAKE_C_STANDARD 17)

find_package(SDL3 REQUIRED CONFIG)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(jinx ${SOURCES})

target_link_libraries(jinx PRIVATE SDL3::SDL3 m)
