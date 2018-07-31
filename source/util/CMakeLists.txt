cmake_minimum_required(VERSION 3.8)

project(starruler2_util)

set(STARRULER2_UTIL_SRC
	source/image.cpp
	source/str_util.cpp
	source/bilinear.cpp
	source/Vector.cpp
)

add_definitions(-std=c++11 -Wall -Wno-invalid-offsetof -Wno-switch -Wno-reorder)

add_library(starruler2_util STATIC ${STARRULER2_UTIL_SRC})
target_link_libraries(starruler2_util ${PNG_LIBRARIES})
target_include_directories(starruler2_util PUBLIC
	${PNG_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
