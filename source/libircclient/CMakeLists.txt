cmake_minimum_required(VERSION 3.8)

project(ircclient)

find_package(Threads)

file(GLOB IRCCLIENT_INCLUDES include)
file(GLOB_RECURSE IRCCLIENT_SOURCES src/*)

configure_file(config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)

add_definitions(-DENABLE_THREADS -DHAVE_LOCALTIME_R)

add_library(ircclient SHARED ${IRCCLIENT_SOURCES})
target_include_directories(ircclient PUBLIC ${IRCCLIENT_INCLUDES})
