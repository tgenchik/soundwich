#include "player_controller/PlayerController.hpp"

#include <iostream>
#include <thread>
#include <vector>

namespace soundwich
{
static constexpr size_t BUFFER_SIZE = 4096;

PlayerController::PlayerController(PlaylistManager &pm, DecoderFactory &df, PipeWireCore &out,
                                   std::ostream &outs, std::ostream &err)
    : playlist(pm), decoderFactory(df), audio(out), out(outs), err(err)
{
    try
    {
        audioOutput = &audio.createPipeWireOutput();
    }
    catch (...)
    {
        err << "Can't create PipeWireOutput\n";
        return;
    }

    playbackThread = std::thread([this]()
                                 {
            while (!end)
            {
                if (audioOutput->getState() != waiting)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    continue;
                }
                try
                {
                    this->nextTrack();
                }
                catch (...)
                {
                }
            } });

    playbackThread.detach();
}


PlayerController::~PlayerController()
{
    end = true;
    playbackThread.join();
}


void PlayerController::play()
{
    if (audioOutput->getState() == playing)
    {
        out << "Track already play\n";
        return;
    }

    TrackPath track;
    try
    {
        track = playlist.getCurrentTrack();
    }
    catch (...)
    {
        err << "No current track" << "\n";
        return;
    }

    try
    {
        decoder = decoderFactory.createDecoder(track.value());
    }
    catch (const std::exception &e)
    {
        err << "Decoder error: " << e.what() << "\n";
        return;
    }

    audioOutput->start();

    std::vector<float> buffer(BUFFER_SIZE);
    while (size_t readSamples = decoder->readSamples(buffer))
    {
        buffer.resize(readSamples);
        audioOutput->writeSamples(buffer);
        buffer.resize(BUFFER_SIZE);
    }

    out << "Playing: " << track->string() << "\n";
}

void PlayerController::pause()
{
    if (audioOutput->getState() == paused)
    {
        out << "Track already pause\n";
        return;
    }
    if (audioOutput->getState() != playing)
    {
        out << "No current track";
        return;
    }
    if (audioOutput)
    {
        audioOutput->pause();
        out << "Paused\n";
        return;
    }
    out << "No current track\n";
}

void PlayerController::stop()
{
    if (audioOutput->getState() == created)
    {
        out << "Track already stop\n";
        return;
    }
    if (audioOutput)
    {
        audioOutput->stop();
        out << "Stopped\n";
        return;
    }
    out << "No current track\n";
}


void PlayerController::resume()
{
    if (audioOutput->getState() == playing)
    {
        out << "Track already play\n";
        return;
    }
    if (audioOutput->getState() != waiting)
    {
        out << "No current track\n";
        return;
    }
    if (audioOutput)
    {
        audioOutput->resume();
        out << "Resumed\n";
    }
    out << "No current track\n";
}

void PlayerController::nextTrack()
{
    try
    {
        playlist.getNextTrack();
        play();
    }
    catch (...)
    {
        err << "Next track is not exist\n";
    }
}

void PlayerController::prevTrack()
{
    try
    {
        playlist.getPrevTrack();
        play();
    }
    catch (...)
    {
        err << "Previous track is not exist\n";
    }
}

void PlayerController::printPlaylistNames() const
{
    out << "==== Playlists ====\n";
    auto names = playlist.getPlaylistNames();
    for (const auto &name : names)
    {
        out << name << "\n";
    }
    out << "===================\n";
}

void PlayerController::printTrackNames() const
{
    std::vector<TrackPath> names;
    try
    {
        names = playlist.getCurrentTracksList();
    }
    catch (...)
    {
        out << "Can't get tracks list\n";
        return;
    }

    out << "===== Tracks =====\n";
    for (const auto &name : names)
    {
        if (name.has_value())
        {
            out << name.value().string() << "\n";
        }
    }
    out << "==================\n";
}

void PlayerController::printTrackInfo() const
{
    out << "==== Now Playing ====\n";

    if (decoder == nullptr)
    {
        out << "No current track\n";
    }
    else
    {
        const AudioMetadata &data = decoder->getMetadata();
        if (data.title.has_value())
        {
            out << "Title:  " << data.title.value() << "\n";
        }
        if (data.artist.has_value())
        {
            out << "Artist:  " << data.artist.value() << "\n";
        }
        if (data.album.has_value())
        {
            out << "Album:  " << data.album.value() << "\n";
        }
        if (data.genre.has_value())
        {
            out << "Genre:  " << data.genre.value() << "\n";
        }
        out << "Duration:  " << data.duration << "\n";
    }

    out << "=====================\n";
}

} // namespace soundwich
