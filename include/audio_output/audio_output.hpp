#pragma once
#include "../common.hpp"
#include <memory>
#include <span>
#include <deque>
#include "IAudioOutput.hpp"
#include <list>
#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>
#include <algorithm>
#include <vector>
#include <mutex>


namespace soundwich
{
class PipeWireCore;


class PipeWireOutput : public IAudioOutput
{
    friend class PipeWireCore;
    friend void process(void* core_ptr);
    
public:
    void start() override;
    void pause() override;
    void resume() override;
    void stop() override;
    void changeSettings(uint32_t newRate, uint32_t newChannels);
    IAudioOutput& writeSamples(std::vector<float> sample) override;
    audioState getState() override;
    float getNext(); //TODO: can make better
    void setVolume(float newVol);

    void dieOnEnd();

    ~PipeWireOutput();

private:
    PipeWireOutput();

    int ind;
    PipeWireCore* core;

    audioState state;

    std::deque<float> data;
    double timer;
    float volume;

    bool dieOnEnd_flag;
};

class PipeWireCore
{
    friend void process(void* core_ptr);
    friend class PipeWireOutput;

public:
    PipeWireCore();
    PipeWireOutput& createPipeWireOutput();
    ~PipeWireCore();

    void changeSettings(uint32_t newRate, uint32_t newChannels);

private:
    pw_thread_loop* loop;
    pw_stream* stream;
    pw_stream_events stream_events;
    std::array<uint8_t, 1024> pod_buffer;
    std::array<PipeWireOutput*, 4096> items;
    std::array<std::mutex, 4096> muts;
    uint32_t rate;
    uint32_t channels;
};

void process(void* core_ptr);
}
