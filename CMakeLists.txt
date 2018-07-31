cmake_minimum_required(VERSION 3.9)
project(starruler2 VERSION 1.0.0 LANGUAGES C CXX)

# This project can use C11, but will gracefully decay down to C89.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)

# This project requires C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(STARRULER2_BUILD_ANGELSCRIPT "Build AngelScript library" ON)

# Location of additional cmake modules
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_MODULE_PATH}"
    )

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(CURL REQUIRED)
find_package(GLFW REQUIRED 3.0.0)
find_package(OpenAL REQUIRED)
find_package(OGG REQUIRED)
find_package(Vorbis REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)

if (UNIX)
    add_definitions(-DLIN_MODE=1 -DNSTEAM=1)
endif (UNIX)

include(BuildTypeConfig)

add_subdirectory(source/libircclient)
add_subdirectory(source/util)
add_subdirectory(source/os)
add_subdirectory(source/network)
add_subdirectory(source/as_addons)
add_subdirectory(source/sound)
add_subdirectory(source)
if(STARRULER2_BUILD_ANGELSCRIPT)
    add_subdirectory(source/angelscript)
endif(STARRULER2_BUILD_ANGELSCRIPT)
