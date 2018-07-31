cmake_minimum_required(VERSION 3.8)

project(starruler2_as_addons)

set(STARRULER2_AS_ADDONS_SRC
	source/scriptany.cpp
	source/scriptarray.cpp
	source/scriptdictionary.cpp
	source/scriptmap.cpp
	source/scripthandle.cpp
	source/scriptmath.cpp
	source/scriptstdstring.cpp
	source/scriptstdstring_utils.cpp
	source/scripthelper.cpp
)

add_definitions(-std=c++11 -Wall -Wno-invalid-offsetof -Wno-switch -Wno-reorder)

add_library(starruler2_as_addons STATIC ${STARRULER2_AS_ADDONS_SRC})
target_link_libraries(starruler2_as_addons starruler2_os AngelScript)
target_include_directories(starruler2_as_addons PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../angelscript/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
