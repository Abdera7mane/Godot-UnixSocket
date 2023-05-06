#!python

import api_generator

ARGUMENTS["custom_api_file"] = "./custom_api.json"

env = SConscript("godot-cpp/SConstruct")
env.Tool("compilation_db")

compile_commands = env.CompilationDatabase("compile_commands.json")
Alias('compile_commands', compile_commands)

library = "libunixsocket"

platform = env["platform"]
target = env["target"]

if not platform in ("linux", "macos"):
    print("No valid target platform selected.")
    quit()

env.Append(CPPPATH=["src"])

sources = ["src/lib.cpp", "src/stream_peer_unix.cpp"]

if platform == "macos":
    library_file = f"{library}.{platform}.{target}"
    library = env.SharedLibrary(f"bin/{library_file}.framework/{library_file}",
                                source=sources)

else:
    suffix = env["suffix"]
    shlib_suffix = env["SHLIBSUFFIX"]
    library = env.SharedLibrary(f"bin/{library}{suffix}{shlib_suffix}",
                                source=sources)

Default(library)

