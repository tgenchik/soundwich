#include "audio_output/audio_output.hpp"
#include <iostream>
#include <chrono>

using namespace soundwich;

void PipeWireOutput::start()
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    if (state == created) state = playing;
}

void PipeWireOutput::pause()
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    if (state == playing) state = paused;
}

void PipeWireOutput::resume()
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    if (state == paused) state = playing;
}

void PipeWireOutput::stop()
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    state = created;
    data.clear();
}

float PipeWireOutput::getNext()
{
    if (data.empty()) return 0;
    float res = *data.begin();
    data.pop_front();
    return res;
}

audioState PipeWireOutput::getState()
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    return state;
}

IAudioOutput& PipeWireOutput::writeSamples(std::vector<float> sample)
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    // auto start = std::chrono::high_resolution_clock::now();

    std::ranges::copy(sample, std::back_inserter(data));

    // auto end = std::chrono::high_resolution_clock::now();
    // auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "copy time: " << milliseconds << '\n';
    return static_cast<IAudioOutput&>(*this);
}

PipeWireOutput::PipeWireOutput()
{
    state = created;
    timer = 0;
    volume = 1;
    dieOnEnd_flag = false;
}

PipeWireOutput::~PipeWireOutput()
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    state = dead;
    core->items[ind] = nullptr;
    data.clear();
}

void PipeWireOutput::setVolume(float newVal)
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    volume = newVal;
}

void PipeWireOutput::dieOnEnd()
{
    // const std::lock_guard<std::mutex> lock(core->muts[ind]);
    dieOnEnd_flag = true;
}
