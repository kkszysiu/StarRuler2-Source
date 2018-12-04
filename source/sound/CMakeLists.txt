cmake_minimum_required(VERSION 3.8)

project(starruler2_sound)

set(STARRULER2_SOUND_SRC
    source/CSound.cpp
	source/CSoundDevice.cpp
	source/CSoundSource.cpp
	source/CStreamSound.cpp
	source/CSSOgg.cpp
	source/CSSWave.cpp
	source/IAudioReference.cpp
	source/ISound.cpp
	source/ISoundDevice.cpp
	source/ISoundSource.cpp
)

add_definitions(-std=c++11 -Wall -Wno-invalid-offsetof -Wno-switch -Wno-reorder)

message("OPENAL_LIBRARY: ${OPENAL_LIBRARY}")
message("OPENAL_INCLUDE_DIR: ${OPENAL_INCLUDE_DIR}")
message("OGG_LIBRARIES: ${OGG_LIBRARIES}")
message("OGG_INCLUDE_DIRS: ${OGG_INCLUDE_DIRS}")
message("VORBIS_LIBRARIES: ${VORBIS_LIBRARIES}")
#message("VORBISENC_LIBRARIES: ${VORBISENC_LIBRARIES}")
#message("VORBISFILE_LIBRARIES: ${VORBISFILE_LIBRARIES}")
message("VORBIS_INCLUDE_DIRS: ${VORBIS_INCLUDE_DIRS}")

find_library(VORBIS_LIBRARY NAMES vorbis)
find_library(VORBISFILE_LIBRARY NAMES vorbisfile)
find_library(VORBISENC_LIBRARY NAMES vorbisenc)

set(VORBIS_LIBRARIES ${VORBISENC_LIBRARY} ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY} ${OGG_LIBRARY})
message("VORBIS_LIBRARIES: ${VORBIS_LIBRARIES}")

add_library(starruler2_sound SHARED ${STARRULER2_SOUND_SRC})
target_link_libraries(starruler2_sound
	${OPENAL_LIBRARY}
	${OGG_LIBRARIES}
	${VORBIS_LIBRARIES}
	${VORBISFILE_LIBRARIES}
	${VORBISENC_LIBRARIES}
	starruler2_os
)
target_include_directories(starruler2_sound PUBLIC
	${OPENAL_INCLUDE_DIR}
	${OGG_INCLUDE_DIRS}
	${VORBIS_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../os/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/OpenALSoft
)
