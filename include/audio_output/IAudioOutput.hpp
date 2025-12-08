#pragma once
#include <span>
#include <memory>
#include <vector>

namespace soundwich
{
enum audioState
{
    created,
    paused,
    playing,
    dead
};

class IAudioOutput
{
public:
    virtual ~IAudioOutput() = default;
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
    virtual IAudioOutput& writeSamples(std::vector<float> sample) = 0;
    virtual audioState getState() = 0;
};


using IAudioOutputPtr = std::unique_ptr<IAudioOutput>;
}
