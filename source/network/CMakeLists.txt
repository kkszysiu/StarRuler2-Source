cmake_minimum_required(VERSION 3.8)

project(starruler2_network)

# set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
# set(THREADS_PREFER_PTHREAD_FLAG TRUE)

# find_package(Threads REQUIRED)

set(STARRULER2_NETWORK_SRC
    source/address.cpp
	source/message.cpp
	source/time.cpp
	source/init.cpp
	source/transport.cpp
	source/message_handler.cpp
	source/connection.cpp
	source/sequence.cpp
	source/server.cpp
	source/client.cpp
	source/lobby.cpp
)

add_definitions(-std=c++11 -Wall -Wno-invalid-offsetof -Wno-switch -Wno-reorder)

add_library(starruler2_network STATIC ${STARRULER2_NETWORK_SRC})
target_link_libraries(starruler2_network starruler2_os)
target_include_directories(starruler2_network PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../os/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
