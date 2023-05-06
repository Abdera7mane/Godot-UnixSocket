#ifndef STREAM_UNIX_H
#define STREAM_UNIX_H

#include <sys/un.h>

#include <godot_cpp/classes/stream_peer_extension.hpp>
#include <godot_cpp/core/binder_common.hpp>

class StreamPeerUnix : public godot::StreamPeerExtension {
    GDCLASS(StreamPeerUnix, StreamPeerExtension)

	bool blocking;

protected:
	int socketfd = -1;
	godot::String path;

	struct sockaddr_un server_address;

  static void _bind_methods();

public:
	godot::Error _get_data(uint8_t *p_buffer, int32_t p_bytes, int32_t *r_received) override;
  godot::Error _get_partial_data(uint8_t *p_buffer, int32_t p_bytes, int32_t *r_received) override;
  godot::Error _put_data(const uint8_t *p_data, int32_t p_bytes, int32_t *r_sent) override;
  godot::Error _put_partial_data(const uint8_t *p_data, int32_t p_bytes, int32_t *r_sent) override;
	int32_t _get_available_bytes() const override;

	int open(const godot::String path);
  godot::String get_path() const;
	bool is_open() const;
	void close();

	void set_blocking_mode(const bool value);
	bool is_blocking_mode_enabled() const;

	~StreamPeerUnix();
	
};

#endif // STREAM_UNIX_H
