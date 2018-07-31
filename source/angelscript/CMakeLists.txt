cmake_minimum_required(VERSION 3.8)

project(angelscript)

file(READ include/angelscript.h ANGELSCRIPT_H)
string(REGEX MATCH "#define ANGELSCRIPT_VERSION_STRING \"([0-9]*).([0-9]*).([0-9]*)" ANGELSCRIPT_VERSION_REGEX ${ANGELSCRIPT_H})
set(ANGELSCRIPT_VERSION_MAJOR ${CMAKE_MATCH_1})
set(ANGELSCRIPT_VERSION_MINOR ${CMAKE_MATCH_2})
set(ANGELSCRIPT_VERSION_PATCH ${CMAKE_MATCH_3})
set(PROJECT_VERSION ${ANGELSCRIPT_VERSION_MAJOR}.${ANGELSCRIPT_VERSION_MINOR}.${ANGELSCRIPT_VERSION_PATCH})

find_package(Threads)

file(GLOB AngelScript_INCLUDES include)
file(GLOB_RECURSE AngelScript_SOURCES source/*)

add_library(AngelScript ${AngelScript_SOURCES})
target_include_directories(AngelScript PUBLIC ${AngelScript_INCLUDES})
set_property(TARGET AngelScript PROPERTY CXX_STANDARD 14)

add_subdirectory(addons)
