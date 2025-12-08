#pragma once
#include "audio_output/audio_output.hpp"
#include "decoder/DecoderFactory.hpp"
#include "playlist_manager.hpp"
#include "common.hpp"
#include <thread>
#include <ostream>

namespace soundwich {
    class PlayerController {
    public:
        PlayerController(PlaylistManager &pm, DecoderFactory &df, PipeWireCore &out, std::ostream& outs, std::ostream& err);

        void play();

        void pause();

        void resume();

        void nextTrack();

        void prevTrack();

        void printTrackInfo();

    private:
        PlaylistManager &playlist;
        DecoderFactory &decoderFactory;
        PipeWireCore &audio;

        std::ostream& out;
        std::ostream& err;

        PipeWireOutput *audioOutput = nullptr;
        std::unique_ptr<IDecoder> decoder;

        std::thread playbackThread;
        bool isPaused = false;
        bool isPlaying = false;
    };
} // namespace soundwich
