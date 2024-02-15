#!python

import json

classes = []

expose = {
    "StreamPeer",
    "StreamPeerExtension",

    # Special cases
    "Object",
    "RefCounted",
    "FileAccess",
    "WorkerThreadPool",
    "XMLParser",
    "ClassDB",
}

singletons = [
    {
        "name": "ClassDB",
        "type": "ClassDB",
    },
]

api = {}
with open("godot-cpp/gdextension/extension_api.json") as file:
    api = json.load(file)

iterator = iter(api["classes"])
while len(expose) > 0:
    clazz = next(iterator)
    class_name = clazz["name"]
    if class_name in expose:
        print(f"Exposed class: {class_name}")
        classes.append(clazz)
        expose.remove(class_name)

api["classes"] = classes
api["singletons"] = singletons

with open("custom_api.json", 'w') as file:
    json.dump(api, file, indent='\t')

