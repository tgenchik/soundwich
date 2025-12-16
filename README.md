# 🎧 Soundwich Console Player

A simple console-based audio player with playlist management, decoding, and PipeWire audio output.

### 📌 Features

- Audio playback via PipeWire
- Playback controls: play / pause / resume
- Track navigation: next / previous
- Add tracks to a playlist
- Automatic playlist load/save
- Display metadata of the current track

### 🚀 Build & Run
```
mkdir build
cd build
cmake ..
make -j
./soundwich
```

### 🧩 Dependencies

soundwich is a C++20 application and depends on the following tools and libraries to build and run correctly:

#### 🛠 Build Requirements

- **CMake ≥ 3.29**
  
  Used as the build system generator.

- **C++20-compatible compiler**

  For example, GCC 11+, Clang 14+, or newer.

- **pkg-config**

  Required to locate system libraries such as libsndfile and PipeWire.

#### 🔊 Runtime / Link-Time Dependencies

- **PipeWire (libpipewire-0.3)**

  Used for audio output and interaction with the PipeWire audio server.

- **libsndfile**

  Provides decoding and reading of audio files.

- **nlohmann/json ≥ 3.12.0**

  Header-only JSON library used for configuration and data serialization.


### 💻 Console Commands

After launching the program, the following commands are available:

```
=== Soundwich Console Player ===
Commands:
  create <playlist name>
  select <playlist name>
  add <track path>
  playlists
  tracks
  info
  play
  pause
  resume
  stop
  next
  prev
  exit
```


### 📄 Command Descriptions

| Command     | 	Description                        |
|-------------|-------------------------------------|
| create \<playlist name> | 	Create a new playlist                 |
| select \<playlist name> | 	Select an existing playlist           |
| add \<path> | 	Add an audio file to the playlist  |
| playlists                | 	List all available playlists          |
| tracks                   | 	List tracks in the current playlist   |
| info        | 	Show metadata of the current track |
| play        | Start playing the current track     |
| pause       | 	Pause playback                     |
| resume      | 	Resume playback                    |
| stop                     | 	Stop playback                         |
| next        | 	Skip to the next track             |
| prev        | 	Go back to the previous track      |
| exit        | 	Exit the application               |

  


