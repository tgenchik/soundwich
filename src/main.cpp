#include "player_controller/PlayerController.hpp"
#include "audio_output/audio_output.hpp"
#include "decoder/DecoderFactory.hpp"
#include "playlist_manager/PlaylistManager.hpp"
#include <iostream>
using namespace soundwich;


int main()
{
    const std::filesystem::path playlist_path("playlist.json");
    PlaylistManager playlist(playlist_path);
    DecoderFactory decoders;
    PipeWireCore audio;
    PlayerController player(playlist, decoders, audio, std::cout, std::cout);

    std::string cmd;

    std::cout << "=== Soundwich Console Player ===\n";
    std::cout << "Commands:\n";
    std::cout << "  create <playlist name>\n";
    std::cout << "  select <playlist name>\n";
    std::cout << "  add <track path>\n";
    std::cout << "  playlists\n";
    std::cout << "  tracks\n";
    std::cout << "  info\n";
    std::cout << "  play\n";
    std::cout << "  pause\n";
    std::cout << "  resume\n";
    std::cout << "  stop\n";
    std::cout << "  next\n";
    std::cout << "  prev\n";
    std::cout << "  exit\n";

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, cmd);

        if (cmd.rfind("create ", 0) == 0)
        {
            std::string name = cmd.substr(7);
            playlist.createPlaylist(name);
            std::cout << "Create playlist: " << name << "\n";
        }
        else if (cmd.rfind("select ", 0) == 0)
        {
            std::string name = cmd.substr(7);
            playlist.selectPlaylist(name);
            std::cout << "Current playlist: " << playlist.getCurrentPlaylistName() << "\n";
        }
        else if (cmd.rfind("add ", 0) == 0)
        {
            std::string spath = cmd.substr(4);
            std::filesystem::path path(spath);
            playlist.addTrack(path);
            std::cout << "Added: " << path << "\n";
        }
        else if (cmd == "playlists")
        {
            player.printPlaylistNames();
        }
        else if (cmd == "tracks")
        {
            player.printTrackNames();
        }
        else if (cmd == "info")
        {
            player.printTrackInfo();
        }
        else if (cmd == "play")
        {
            player.play();
        }
        else if (cmd == "pause")
        {
            player.pause();
        }
        else if (cmd == "resume")
        {
            player.resume();
        }
        else if (cmd == "stop")
        {
            player.stop();
        }
        else if (cmd == "next")
        {
            player.nextTrack();
        }
        else if (cmd == "prev")
        {
            player.prevTrack();
        }
        else if (cmd == "exit")
        {
            break;
        }
        else
        {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}
