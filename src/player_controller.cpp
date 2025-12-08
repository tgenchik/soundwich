#include "player_controller.hpp"

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

namespace soundwich {
    static constexpr size_t BUFFER_SIZE = 4096;

    PlayerController::PlayerController(PlaylistManager &pm, DecoderFactory &df, PipeWireCore &out,
                                       std::ostream &outs, std::ostream &err)
        : playlist(pm), decoderFactory(df), audio(out), out(outs), err(err) {}

    void PlayerController::play() {
        if (isPlaying) {
            pause();
        }

        const Track &track = playlist.getCurrentTrack();

        try {
            decoder = decoderFactory.createDecoder(track.path);
        } catch (const std::exception &e) {
            err << "Decoder error: " << e.what() << "\n";
            return;
        }

        audioOutput = &audio.createPipeWireOutput();
        audioOutput->start();

        isPaused = false;
        isPlaying = true;

        playbackThread = std::thread([this]() {
            std::vector<float> buffer(BUFFER_SIZE);

            while (isPlaying) {
                if (isPaused) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    continue;
                }

                size_t readSamples = decoder->readSamples(buffer);

                if (readSamples == 0) {
                    playlist.nextTrack();
                    this->play();
                    return;
                }

                buffer.resize(readSamples);
                audioOutput->writeSamples(buffer);
                buffer.resize(BUFFER_SIZE);
            }
        });

        playbackThread.detach();

        out << "Playing: " << track.title << "\n";
    }

    void PlayerController::pause() {
        if (!isPlaying || isPaused) {
            return;
        }

        isPaused = true;
        if (audioOutput) {
            audioOutput->pause();
        }

        out << "Paused\n";
    }

    void PlayerController::resume() {
        if (!isPlaying || !isPaused) {
            return;
        }

        isPaused = false;
        if (audioOutput) {
            audioOutput->resume();
        }

        out << "Resumed\n";
    }

    void PlayerController::nextTrack() {
        try {
            playlist.nextTrack();
        } catch (SoundwichError e) {
            err << "Next track is not exist\n";
        }
        play();
    }

    void PlayerController::prevTrack() {
        try {
            playlist.prevTrack();
        } catch (SoundwichError e) {
            err << "Previous track is not exist\n";
        }
        play();
    }

    void PlayerController::printTrackInfo() {
        const AudioMetadata& data = decoder->getMetadata();
        out << "\n=== Now Playing ===\n";
        if (data.title.has_value()) {
            out << "Title:  " << data.title.value() << "\n";
        }
        if (data.artist.has_value()) {
            out << "Artist:  " << data.artist.value() << "\n";
        }
        if (data.album.has_value()) {
            out << "Album:  " << data.album.value() << "\n";
        }
        if (data.genre.has_value()) {
            out << "Genre:  " << data.genre.value() << "\n";
        }
        out << "Duration:  " << data.duration << "\n";
        out << "====================\n";
    }

} // namespace soundwich
