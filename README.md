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
  add <path>
  play
  pause
  resume
  next
  prev
  info
  exit
```

### 📄 Command Descriptions

| Command     | 	Description                        |
|-------------|-------------------------------------|
| add \<path> | 	Add an audio file to the playlist  |
| play        | Start playing the current track     |
| pause       | 	Pause playback                     |
| resume      | 	Resume playback                    |
| next        | 	Skip to the next track             |
| prev        | 	Go back to the previous track      |
| info        | 	Show metadata of the current track |
| exit        | 	Exit the application               |

