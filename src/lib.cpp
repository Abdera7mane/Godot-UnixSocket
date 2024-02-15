#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>

#include "stream_peer_unix.h"

extern "C" GDExtensionBool GDE_EXPORT
unixsocket_library_init(GDExtensionInterfaceGetProcAddress p_proc_address,
                        GDExtensionClassLibraryPtr p_library,
                        GDExtensionInitialization *r_initialization) {
  godot::GDExtensionBinding::InitObject init_object(p_proc_address, p_library,
                                                    r_initialization);

  const auto min_level = godot::MODULE_INITIALIZATION_LEVEL_CORE;
  init_object.set_minimum_library_initialization_level(min_level);
  init_object.register_initializer([](auto level) {
    if (level == min_level) godot::ClassDB::register_class<StreamPeerUnix>();
  });

  return init_object.init();
}
