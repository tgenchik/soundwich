#include "audio_output/audio_output.hpp"
#include <iostream>
#include <chrono>

using namespace soundwich;

void PipeWireOutput::start()
{
    if (state == created) state = playing;
}

void PipeWireOutput::pause()
{
    if (state == playing) state = paused;
}

void PipeWireOutput::resume()
{
    if (state == paused) state = playing;
}

void PipeWireOutput::stop()
{
    state = created;
    data.clear();
}

float PipeWireOutput::getNext()
{
    if (data.empty())
    {
        state = waiting;
        return 0;
    }
    float res = *data.begin();
    data.pop_front();
    return res;
}

audioState PipeWireOutput::getState()
{
    return state;
}

IAudioOutput& PipeWireOutput::writeSamples(std::vector<float> sample)
{
    if (state == waiting) state = playing;
    std::ranges::copy(sample, std::back_inserter(data));
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
    state = dead;
    core->items[ind] = nullptr;
    data.clear();
}

void PipeWireOutput::setVolume(float newVal)
{
    volume = newVal;
}

void PipeWireOutput::dieOnEnd()
{
    dieOnEnd_flag = true;
}
