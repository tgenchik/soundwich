#pragma once
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>


namespace soundwich
{

struct SoundwichError : public std::runtime_error
{
    explicit SoundwichError(const std::string &msg) : std::runtime_error(msg) {}
};

using TrackPath = std::optional<std::filesystem::path>;

} // namespace soundwich
