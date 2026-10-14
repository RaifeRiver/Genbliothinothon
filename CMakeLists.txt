cmake_minimum_required(VERSION 4.4)

project(Genbliothinothon)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
add_executable(Genbliothinothon ${SRC_FILES})

target_include_directories(Genbliothinothon PUBLIC src)