#pragma once
#include "common.hpp"
#include <string>
#include <unordered_map>
#include <vector>


namespace soundwich
{

struct PlaylistManagerError : public SoundwichError
{
    explicit PlaylistManagerError(const std::string &msg) : SoundwichError(msg) {}
};

class PlaylistManager
{
public:
    PlaylistManager();

    // Reads config from a file or creates an empty one if it doesn't exist.
    PlaylistManager(std::string_view configJson);
    PlaylistManager(const std::filesystem::path &configJson);

    void loadJsonConfig(std::string_view str);

    // Writes a json string that contains whole configuration.
    void dumpJsonConfig(std::string &str) const;
    void dumpJsonConfig(std::ostream &out) const;
    void dumpJsonConfig(const std::filesystem::path &path) const;

    std::string getCurrentPlaylistName() const;

    void setCurrentPlaylistName(std::string_view newName);

    // Selects a playlist, creating it if needed.
    void selectPlaylist(std::string_view playlist);

    // Creates a new playlist `name`, doesn't select it.
    void createPlaylist(std::string_view name);

    std::vector<std::string> getPlaylistNames() const;

    // Add a track to the end of the current playlist.
    void addTrack(const std::filesystem::path &path);

    void deleteTrackByIndex(std::size_t index);

    const TrackPath getCurrentTrack() const;

    const std::vector<TrackPath> &getCurrentTracksList() const;

    // Selects next track and returns it.
    TrackPath getNextTrack();

    // Selects prev track and returns it.
    TrackPath getPrevTrack();

private:
    struct Playlist
    {
        std::vector<TrackPath> tracks;
    };

    std::unordered_map<std::string, Playlist> m_playlists;
    std::string m_current_playlist;
    std::size_t m_current_track;
};

} // namespace soundwich
