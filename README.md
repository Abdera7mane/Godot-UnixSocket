# Godot-UnixSocket

**GDNative** module for Unix sockets communications in [Godot Engine](https://github.com/godotengine/godot) under Linux and MacOS builds.

## Compiling
1. Install [Scons]() (`v3.0.1` and above is required)  
	Linux: `python -m pip install scons`  
	MacOS: `brew install scons`
2. Clone this repository with:  
	1. `git clone --recursive https://github.com/Abdera7mane/Godot-UnixSocket.git`
	2. `cd Godot-UnixSocket`
	3. `git submodule update --init --recursive`
3. Run the approperiate script according to your system to build the library:  
	Linux: `./build_linux.sh`  
	MacOS: `./build_mac.sh`
4. The binaries should be generated in `bin/` directory.

## Documentation

### class StreamPeerUnix
**Inherits:**
[StreamPeerGDNative](https://docs.godotengine.org/en/stable/classes/class_streampeergdnative.html#class-streampeergdnative) <
[StreamPeer](https://docs.godotengine.org/en/stable/classes/class_streampeer.html)

### properties

| type | name | default | description                                                        |
|------|------|---------|--------------------------------------------------------------------|
| int  | type | 0       | socket type, ( 0: SOCK_STREAM, 1: SOCK_DGRAM, 2: SOCK_SEQPACKET )  |

### methods

| return | definition         | description                                                        |
|--------|--------------------|--------------------------------------------------------------------|
| int    | open(path: String) | Opens a connection to a socket at `path` and returns an error code |
| bool   | is_open()          | Checks if the stream peer is connected                             |
| String | get_path()         | Gets the current connected socket path                             |
| void   | close()            | Closes the socket connection                                       |