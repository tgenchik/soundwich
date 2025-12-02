#pragma once

#include "decoder/IDecoder.hpp"

#include "sndfile.h"

namespace soundwich
{

// A decoder based on libsndfile.
class SndfileDecoder : public IDecoder
{
public:
    SndfileDecoder(const std::filesystem::path &path);

    ~SndfileDecoder() override;

    const AudioMetadata &getMetadata() const override;
    std::size_t readSamples(std::span<float> buffer) override;
    std::size_t position() const override;
    std::size_t seek(std::size_t frameOffset, int origin) override;

private:
    struct Deleter
    {
        void operator()(SNDFILE *file);
    };

    std::unique_ptr<SNDFILE, Deleter> mFile;
    SF_INFO mInfo;
    AudioMetadata mMetadata;
};

} // namespace soundwich
