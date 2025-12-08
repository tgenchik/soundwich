/*
пояснение к main
сейчас программа читает все нажатые клавиши как строку и играет такие ноту
q - до первой октавы
2 - чёрная после неё (до диез)
и т.д.
для теста можете попробовать ввести qet или q3t
*/


#include "audio_output/audio_output.hpp"
#include "decoder_manager.hpp"
// #include "player_controller.hpp"
#include "playlist_manager.hpp"
#include <iostream>
#include <thread>
#include <map>

using namespace soundwich;

std::map<char, int> key_to_note = {
        {'q', 0 },
        {'2', 1 },
        {'w', 2 },
        {'3', 3 },
        {'e', 4 },
        {'r', 5 },
        {'5', 6 },
        {'t', 7 },
        {'6', 8 },
        {'y', 9 },
        {'7', 10},
        {'u', 11},

        {'i', 12},
        {'9', 13},
        {'o', 14},
        {'0', 15},
        {'p', 16},
    };

float get_note(float t, int ind) // даёт частоту ноты по номеру
{
    double q12 = pow((double)2, (double)1 / 12);
    return t * pow(q12, ind);
}

void makeSound(PipeWireCore& core, float freq) // играет звук с нужной частотой
{
    PipeWireOutput* a = &core.createPipeWireOutput(); // новый поток проигрывания
    a->setVolume(0.1); // громкость
    a->dieOnEnd(); // само удаляется в конце исполнения

    double t = 0;
    std::vector<float> test(100000); // массив для звука
    for (int i = 0; i < test.size(); i++)
    {
        t += 2 * M_PI * freq / 44100;
        if (t >= 2 * M_PI) t -= 2 * M_PI;
        test[i] = sin(t);
    }

    a->writeSamples(test);
    a->start();
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // delete a;
    // сам поток остаётся, в него можно ещё писать
    // в нём есть деструктор, но конкретно тут утечка памяти
}

int main()
{
    PipeWireCore core; // ядро отвечает за управление всеми потоками

    std::string a;
    while (1)
    {
        std::cin >> a;
        if (a == "z") break;
        for (char b : a)
            makeSound(core, get_note(440, key_to_note[b] - 9));
    }
}
