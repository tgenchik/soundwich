#pragma once
#include "audio_output/audio_output.hpp"
#include "decoder/DecoderFactory.hpp"
#include "playlist_manager.hpp"


namespace soundwich
{

class PlayerController
{
public:
    PlayerController(PlaylistManager &pm, DecoderFactory &dm, PipeWireCore &out);

    void play();
    void pause();
    void resume();
    void nextTrack();
    void prevTrack();

private:
    PlaylistManager &playlist;
    DecoderFactory &decoderManager;
    PipeWireCore &audio;

    std::unique_ptr<IDecoder> decoder;
    bool isPaused = false;
    bool isPlaying = false;
};

} // namespace soundwich
