cmake_minimum_required(VERSION 3.29)
project("soundwich")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O0")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)
find_package(nlohmann_json 3.12.0 REQUIRED)

include_directories(
        src
        include
)

set(HEADER_FILES
        include/audio_output/audio_output.hpp
        include/audio_output/IAudioOutput.hpp

        include/decoder/DecoderFactory.hpp
        include/decoder/IDecoder.hpp
        include/decoder/SndfileDecoder.hpp

        include/player_controller.hpp
        include/playlist_manager.hpp
        include/common.hpp
)

set(SRC_FILES
        src/audio_output/PipeWireCore.cpp
        src/audio_output/PipeWireOutput.cpp
        src/audio_output/process.cpp

        src/decoder/DecoderFactory.cpp
        src/decoder/SndfileDecoder.cpp

        src/main.cpp
        src/playlist_manager.cpp
        src/player_controller.cpp
)

add_executable(soundwich
        ${SRC_FILES}
        ${HEADER_FILES}
)

target_include_directories(soundwich
        PRIVATE
        ${PIPEWIRE_INCLUDE_DIRS}
        ${SNDFILE_INCLUDE_DIRS}
)

target_link_libraries(soundwich
        PRIVATE
        m
        ${PIPEWIRE_LIBRARIES}
        ${SNDFILE_LIBRARIES}
        nlohmann_json::nlohmann_json
)
