cmake_minimum_required(VERSION 3.6)

if(ANDROID)
string(TOLOWER ${CMAKE_BUILD_TYPE} ANDROID_BUILD_DIR)
set(DISTRIBUTION_DIR ${CMAKE_SOURCE_DIR}/android/distribution/android/SDL2/intermediates/ndkBuild)
set(SDL_LOCATION ${CMAKE_SOURCE_DIR}/external/SDL2)

add_library( SDL2 SHARED IMPORTED )
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION
${DISTRIBUTION_DIR}/${ANDROID_BUILD_DIR}/obj/local/${ANDROID_ABI}/libSDL2.so)

include_directories(${SDL_LOCATION}/SDL2/include)

endif(ANDROID)

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

option(USE_GLFW "Use GLFW library" ON)
option(USE_SDL2 "Use SDL2 library" OFF)

# Location of additional cmake modules
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_MODULE_PATH}"
    )

if(NOT ANDROID)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
elif(NOT ANDROID)
find_package(GLES2 REQUIRED)
endif(NOT ANDROID)
find_package(CURL REQUIRED)
# find_package(OpenAL REQUIRED)
# find_package(OGG REQUIRED)
# find_package(Vorbis REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)

if (UNIX)
    add_definitions(-DLIN_MODE=1 -DNSTEAM=1)
endif (UNIX)

if (USE_GLFW)
    find_package(GLFW REQUIRED 3.0.0)

    set(USE_SDL2 OFF)
    add_definitions(-DUSE_GLFW)
    remove_definitions(-DUSE_SDL2)
endif (USE_GLFW)

if (USE_SDL2)
    find_package(SDL2 REQUIRED)

    set(USE_GLFW OFF)
    add_definitions(-DUSE_SDL2)
    remove_definitions(-DUSE_GLFW)
endif (USE_SDL2)

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
