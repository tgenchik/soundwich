#pragma once
#include "audio_output/audio_output.hpp"
#include "common.hpp"
#include "decoder/DecoderFactory.hpp"
#include "playlist_manager/PlaylistManager.hpp"
#include <ostream>
#include <thread>

namespace soundwich {
    class PlayerController {
    public:
        PlayerController(PlaylistManager &pm, DecoderFactory &df, PipeWireCore &out, std::ostream& outs, std::ostream& err);

        ~PlayerController();

        void play();

        void pause();

        void resume();

        void stop();

        void nextTrack();

        void prevTrack();

        void printPlaylistNames() const;

        void printTrackNames() const;

        void printTrackInfo() const;

    private:
        PlaylistManager &playlist;
        DecoderFactory &decoderFactory;
        PipeWireCore &audio;

        std::ostream &out;
        std::ostream &err;

        PipeWireOutput *audioOutput = nullptr;
        std::unique_ptr<IDecoder> decoder;

        std::thread playbackThread;
        bool isPaused = false;
        bool isPlaying = false;
        bool end = false;
    };
} // namespace soundwich
