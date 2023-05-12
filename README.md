# Godot-UnixSocket

[GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/what_is_gdextension.html) module for UNIX domain sockets support in [Godot Engine](https://github.com/godotengine/godot) under Linux and MacOS builds.

## Build

### Prerequisites

1. [Python](https://www.python.org)
2. [SCons](https://scons.org)
3. C/C++ compiler

### Compiling

1. Clone the repository:
```
git clone --recursive https://github.com/Abdera7mane/Godot-UnixSocket.git
cd Godot-UnixSocket
git submodule update --init --recursive
```
2. Build with scons (outputs to `bin`):  
#### debug build
```
scons target=template_debug
```
#### release build
```
scons target=template_release
```

## Documentation

### class StreamPeerUnix
**Inherits:**
[StreamPeerExtension](https://docs.godotengine.org/en/stable/classes/class_streampeerextension.html) <
[StreamPeer](https://docs.godotengine.org/en/stable/classes/class_streampeer.html) <
[RefCounted](https://docs.godotengine.org/en/stable/classes/class_refcounted.html) <
[Object](https://docs.godotengine.org/en/stable/classes/class_object.html)

### methods

| return                 | definition           | description                                                        |
|------------------------|----------------------|--------------------------------------------------------------------|
| [int][class_int]       | `open(path: String)` | Opens a connection to a socket at `path` and returns an error code |
| [bool][class_bool]     | `is_open()`          | Checks if the stream peer is connected                             |
| [String][class_string] | `get_path()`         | Gets the current connected socket path                             |
| void                   | `close()`            | Closes the socket connection                                       |

[class_int]: https://docs.godotengine.org/en/stable/classes/class_int.html
[class_bool]: https://docs.godotengine.org/en/stable/classes/class_bool.html
[class_string]: https://docs.godotengine.org/en/stable/classes/class_string.html


