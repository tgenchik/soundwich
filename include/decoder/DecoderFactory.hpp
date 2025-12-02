#pragma once

#include <memory>

#include "decoder/IDecoder.hpp"

namespace soundwich
{

class DecoderFactory
{
public:
    // Creates a new file decoder.
    std::unique_ptr<IDecoder> createDecoder(const std::filesystem::path &path);
};

} // namespace soundwich
