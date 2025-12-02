#pragma once

#include <chrono>
#include <filesystem>
#include <optional>
#include <span>

#include "common.hpp"

namespace soundwich
{

struct DecoderError : public SoundwichError
{
    explicit DecoderError(const std::string &msg) : SoundwichError(msg) {}
};

struct AudioMetadata
{
    std::filesystem::path path;
    std::string format;

    std::optional<std::string> copyright;
    std::optional<std::string> license;

    std::optional<std::string> title;
    std::optional<std::string> artist;
    std::optional<std::string> album;
    std::optional<std::string> trackNumber;

    std::optional<std::string> comment;
    std::optional<std::string> genre;
    std::optional<std::string> date;

    std::size_t frames;
    int samplingRateHz;
    std::chrono::seconds duration;
    int channels;
    bool seekable;
};

class IDecoder
{
public:
    IDecoder() = default;
    virtual ~IDecoder() = default;

    IDecoder(const IDecoder &) = delete;
    IDecoder &operator=(const IDecoder &) = delete;

    IDecoder(IDecoder &&) = delete;
    IDecoder &operator=(IDecoder &&) = delete;

    // File metadata.
    virtual const AudioMetadata &getMetadata() const = 0;

    // Reads a maximum of `buffer.size()` samples (not frames). For examples trying to read 8 samples from a file with 2
    // channels will result in 4 frames read with samples interleaved).
    // Returns the number of samples read.
    virtual std::size_t readSamples(std::span<float> buffer) = 0;

    // Current position in frames.
    virtual std::size_t position() const = 0;

    // Sets position according to offset and origin. Offset measured is in frames and might be negative. Therefore, a
    // seek in a stereo file from the current position forward with an offset of 1 would skip forward by one sample of
    // both channels.
    // `origin` must be one of SEEK_SET, SEEK_CUR, SEEK_END.
    //
    // Returns the position in frames.
    virtual std::size_t seek(std::uint64_t frameOffset, int origin) = 0;
};

} // namespace soundwich
