#include "playlist_manager/PlaylistManager.hpp"
#include "common.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

namespace soundwich
{

PlaylistManager::PlaylistManager()
    : m_playlists(), m_current_playlist("default"), m_current_track(0)
{
    createPlaylist(m_current_playlist);
}

PlaylistManager::PlaylistManager(const std::filesystem::path &configJson)
    : PlaylistManager()
{
    try {
        std::ifstream ifs(configJson);
        std::string str(std::istreambuf_iterator<char>{ifs}, {});
        loadJsonConfig(str);
    } catch (...) {}
}

PlaylistManager::PlaylistManager(std::string_view configJson)
{
    loadJsonConfig(configJson);
}

void PlaylistManager::loadJsonConfig(std::string_view str)
{
    auto obj = nlohmann::json::parse(str);

    m_current_playlist = obj["rock"];

    for (auto &playlist : obj["playlists"])
    {
        std::string name = playlist["name"];
        std::vector<TrackPath> songs;
        for (auto &song : playlist["songs"])
        {
            songs.push_back(std::string(song["path"]));
        }

        m_playlists[name].tracks = songs;
    }
}

void PlaylistManager::dumpJsonConfig(std::string &str) const
{
    auto convertTrack = [](const TrackPath &t) -> nlohmann::json
    {
        return nlohmann::json({{"path", t}});
    };

    auto convertPlaylist = [&](const std::vector<TrackPath> &playlist, const std::string &name) -> nlohmann::json
    {
        nlohmann::json songs = nlohmann::json::array();
        for (auto &e : playlist)
        {
            songs.push_back(convertTrack(e));
        }

        return nlohmann::json({{"name", name},
                               {"songs", songs}});
    };

    nlohmann::json playlists = nlohmann::json::array();
    for (auto &[k, v] : m_playlists)
    {
        playlists.push_back(convertPlaylist(v.tracks, k));
    }

    str = nlohmann::json({{"playlists", playlists},
                          {"current_playlist", m_current_playlist}})
              .dump();
}

void PlaylistManager::dumpJsonConfig(std::ostream &out) const
{
    std::string str;
    dumpJsonConfig(str);
    out << str;
}

void PlaylistManager::dumpJsonConfig(const std::filesystem::path &path) const
{
    std::ofstream out(path, std::ios::trunc);
    dumpJsonConfig(out);
}

std::string PlaylistManager::getCurrentPlaylistName() const
{
    return m_current_playlist;
}

void PlaylistManager::setCurrentPlaylistName(std::string_view newName)
{
    if (m_playlists.count(std::string(newName)))
    {
        throw PlaylistManagerError("[PlaylistManager]: playlist with such name already exist");
    }

    if (newName == m_current_playlist)
    {
        return;
    }

    try
    {
        auto data = m_playlists.at(m_current_playlist);
        m_playlists.erase(m_current_playlist);
        m_current_playlist = newName;
        m_playlists.insert({m_current_playlist, data});
    }
    catch (...)
    {
        throw PlaylistManagerError("[PlaylistManager]: failed to rename a playlist");
    }
}

void PlaylistManager::selectPlaylist(std::string_view playlist)
{
    createPlaylist(playlist);
    m_current_playlist = playlist;
}

// Creates a new playlist `name`, doesn't select it.
void PlaylistManager::createPlaylist(std::string_view name)
{
    m_playlists.insert({std::string(name), {}});
}

void PlaylistManager::addTrack(const std::filesystem::path &path)
{
    m_playlists.at(m_current_playlist).tracks.push_back(path);
}

void PlaylistManager::deleteTrackByIndex(std::size_t index)
{
    auto &tracks = m_playlists.at(m_current_playlist).tracks;
    if (index >= tracks.size())
    {
        throw PlaylistManagerError("[PlaylistManager]: index excedes track count");
    }
    tracks.erase(tracks.begin() + index);
}

const TrackPath PlaylistManager::getCurrentTrack() const
{
    const auto &curr = getCurrentTracksList();
    if (m_current_track < curr.size())
    {
        return curr[m_current_track];
    }
    else
    {
        throw PlaylistManagerError("[PlaylistManager]: current track excedes playlist length");
    }
}

const std::vector<TrackPath> &PlaylistManager::getCurrentTracksList() const
{
    try
    {
        return m_playlists.at(m_current_playlist).tracks;
    }
    catch (...)
    {
        throw PlaylistManagerError("[PlaylistManager]: current track excedes playlist length");
    }
}

TrackPath PlaylistManager::getNextTrack()
{
    const auto &curr = getCurrentTracksList();
    if (curr.size() == 0)
    {
        throw PlaylistManagerError("[PlaylistManager]: current playlist size is empty");
    }
    m_current_track = (m_current_track + 1) % curr.size();
    return curr[m_current_track];
}

TrackPath PlaylistManager::getPrevTrack()
{
    const auto &curr = getCurrentTracksList();
    if (curr.size() == 0)
    {
        throw PlaylistManagerError("[PlaylistManager]: current playlist size is empty");
    }
    m_current_track = (m_current_track + curr.size() - 1) % curr.size();
    return curr[m_current_track];
}

} // namespace soundwich
