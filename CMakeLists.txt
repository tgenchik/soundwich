cmake_minimum_required(VERSION 3.29)
project("soundwich")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O0")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)

include_directories(
        src
        include
)

set(HEADER_FILES
        include/decoder/DecoderFactory.hpp
        include/decoder/IDecoder.hpp
        include/decoder/SndfileDecoder.hpp

        include/audio_output.hpp
        include/player_controller.hpp
        include/playlist_manager.hpp
        include/common.hpp
)

set(SRC_FILES
        src/decoder/DecoderFactory.cpp
        src/decoder/SndfileDecoder.cpp

        src/main.cpp
)

add_executable(soundwich
        ${SRC_FILES}
        ${HEADER_FILES}
)

target_include_directories(soundwich PRIVATE ${SNDFILE_INCLUDE_DIRS})
target_link_libraries(soundwich ${SNDFILE_LIBRARIES})
