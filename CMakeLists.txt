cmake_minimum_required(VERSION 4.3.1)

project(Genbliothinothon)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.5.1)
FetchContent_MakeAvailable(glfw)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.3)
FetchContent_MakeAvailable(glm)
FetchContent_Declare(portaudio GIT_REPOSITORY https://github.com/PortAudio/portaudio.git)
FetchContent_MakeAvailable(portaudio)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp lib/*.c)
add_executable(Genbliothinothon ${SRC_FILES})

target_compile_definitions(Genbliothinothon PUBLIC GLM_ENABLE_EXPERIMENTAL)

target_include_directories(Genbliothinothon PUBLIC lib)
target_include_directories(Genbliothinothon PUBLIC src)

target_link_libraries(Genbliothinothon PUBLIC glfw)
target_link_libraries(Genbliothinothon PUBLIC glm)
target_link_libraries(Genbliothinothon PUBLIC portaudio)