#include "decoder/DecoderFactory.hpp"
#include "decoder/SndfileDecoder.hpp"

#include <memory>

namespace soundwich
{

std::unique_ptr<IDecoder>
DecoderFactory::createDecoder(const std::filesystem::path &path)
{
    return std::make_unique<SndfileDecoder>(path);
}

} // namespace soundwich
