#include "decoder/SndfileDecoder.hpp"

namespace soundwich
{

void SndfileDecoder::Deleter::operator()(SNDFILE *file)
{
    if (file != NULL)
    {
        int error_code = sf_close(file);
        if (error_code != 0)
        {
            throw DecoderError(
                "[SndfileDecoder] Failed to close a file, error code: " + std::string(sf_strerror(file)));
        }
    }
}

SndfileDecoder::SndfileDecoder(const std::filesystem::path &path)
    : mFile(sf_open(path.c_str(), SFM_READ, &mInfo))
{
    if (mFile == NULL)
    {
        throw DecoderError(
            "[SndfileDecoder] Failed to open a file: " + std::string(sf_strerror(NULL)));
    }

    SF_FORMAT_INFO formatInfo;
    formatInfo.format = mInfo.format;
    int err = sf_command(mFile.get(), SFC_GET_FORMAT_INFO, &formatInfo, sizeof(formatInfo));
    if (err != SF_ERR_NO_ERROR)
    {
        throw DecoderError(
            "[SndfileDecoder] Failed to retrieve file format info: " + std::string(sf_strerror(mFile.get())));
    }

    auto getField = [&](int field) -> std::optional<std::string>
    {
        const char *str = sf_get_string(mFile.get(), field);
        if (str == NULL)
        {
            return {};
        }
        return std::string(str);
    };

    mMetadata = AudioMetadata{
        .path = path,
        .format = formatInfo.name,

        .copyright = getField(SF_STR_COPYRIGHT),
        .license = getField(SF_STR_LICENSE),

        .title = getField(SF_STR_TITLE),
        .artist = getField(SF_STR_ARTIST),
        .album = getField(SF_STR_ALBUM),
        .trackNumber = getField(SF_STR_TRACKNUMBER),
        .comment = getField(SF_STR_COMMENT),
        .genre = getField(SF_STR_GENRE),
        .date = getField(SF_STR_DATE),

        .frames = static_cast<std::size_t>(mInfo.frames),
        .samplingRateHz = mInfo.samplerate,
        .duration = std::chrono::seconds(mInfo.frames / mInfo.samplerate),
        .channels = mInfo.channels,
        .seekable = (mInfo.seekable != 0)};
}

SndfileDecoder::~SndfileDecoder() = default;

const AudioMetadata &SndfileDecoder::getMetadata() const
{
    return mMetadata;
}

std::size_t SndfileDecoder::readSamples(std::span<float> buffer)
{
    return sf_read_float(mFile.get(), buffer.data(), buffer.size());
}

std::size_t SndfileDecoder::position() const
{
    return sf_seek(mFile.get(), 0, SEEK_CUR);
}

std::size_t SndfileDecoder::seek(std::size_t frameOffset, int origin)
{
    auto pos = sf_seek(mFile.get(), frameOffset, origin);
    if (pos == -1)
    {
        throw DecoderError(
            "[SndfileDecoder] Failed to change position: " + std::string(sf_strerror(mFile.get())));
    }

    return pos;
}

} // namespace soundwich
