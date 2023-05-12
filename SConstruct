#!python

import api_generator

ARGUMENTS["custom_api_file"] = "./custom_api.json"

env = SConscript("godot-cpp/SConstruct")
env.Tool("compilation_db")

compile_commands = env.CompilationDatabase("compile_commands.json")
Alias('compile_commands', compile_commands)

library_name = "libunixsocket"

platform = env["platform"]

if not platform in ("linux", "macos"):
    print("No valid target platform selected.")
    quit()

env.Append(CPPPATH=["src"])

sources = ["src/lib.cpp", "src/stream_peer_unix.cpp"]

target_name = env["target"]

if target_name != "editor":
    target_name = "release" if target_name == "template_release" else "debug"

arch = env["arch"]
suffix = f".{platform}.{target_name}.{arch}"

shlib_suffix = env["SHLIBSUFFIX"]
library = env.SharedLibrary(f"bin/{library_name}{suffix}{shlib_suffix}",
                            source=sources)

Default(library)

