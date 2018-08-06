cmake_minimum_required(VERSION 3.8)

project(starruler2_os)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)

if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
	MESSAGE("++ 64 bit architecture") 
	set(ARCHNAME "x64") 
else() 
	MESSAGE("++ 32 bit architecture") 
	set(ARCHNAME "x86")
endif()

set(STARRULER2_OS_SRC
	source/threads.cpp
	source/threads_gcc.cpp
	source/files.cpp
	source/files_linux.cpp
	source/virtual_asm_linux.cpp
	source/virtual_asm_${ARCHNAME}.cpp
)

add_definitions(-std=c++11 -Wall -Wno-invalid-offsetof -Wno-switch -Wno-reorder -fPIC)

add_library(starruler2_os STATIC ${STARRULER2_OS_SRC})
target_link_libraries(starruler2_os starruler2_util Threads::Threads)
target_include_directories(starruler2_os PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../util/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
