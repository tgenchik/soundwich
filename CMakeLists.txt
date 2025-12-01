cmake_minimum_required(VERSION 3.29)
project("soundwich")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O0")

include_directories(
        src
        include
)

set(HEADER_FILES
        include/audio_output.hpp
        include/decoder_manager.hpp
        include/player_controller.hpp
        include/playlist_manager.hpp
        include/common.hpp
)

set(SRC_FILES
        src/main.cpp
)

add_executable(soundwich
        ${SRC_FILES}
        ${HEADER_FILES}
)
