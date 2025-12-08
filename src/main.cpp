#include "audio_output/audio_output.hpp"
#include "decoder/DecoderFactory.hpp"
#include "player_controller.hpp"
#include "playlist_manager.hpp"
#include <iostream>
using namespace soundwich;


int main()
{
    PlaylistManager playlist("playlist.txt");
    DecoderFactory decoders;
    PipeWireCore audio;
    PlayerController player(playlist, decoders, audio, std::cout, std::cout);

    std::string cmd;

    std::cout << "=== Soundwich Console Player ===\n";
    std::cout << "Commands:\n";
    std::cout << "  add <path>\n";
    std::cout << "  play\n";
    std::cout << "  pause\n";
    std::cout << "  resume\n";
    std::cout << "  next\n";
    std::cout << "  prev\n";
    std::cout << "  info\n";
    std::cout << "  exit\n";

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, cmd);

        if (cmd.rfind("add ", 0) == 0)
        {
            std::string path = cmd.substr(4);
            playlist.addTrack(path);
            std::cout << "Added: " << path << "\n";
        }
        else if (cmd == "play")
        {
            std::cout << "Playing...\n";
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
        else if (cmd == "next")
        {
            playlist.nextTrack();
            player.play();
        }
        else if (cmd == "prev")
        {
            playlist.prevTrack();
            player.play();
        }
        else if (cmd == "info")
        {
            player.printTrackInfo();
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
