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

  


