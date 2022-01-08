#ifndef STREAM_UNIX_H
#define STREAM_UNIX_H

#include <sys/un.h>

#include <Godot.hpp>
#include <StreamPeerGDNative.hpp>

using namespace godot;

godot_error _get_data(void *, uint8_t *, int);
godot_error _get_partial_data(void *, uint8_t *, int, int *);
godot_error _put_data(void *, const uint8_t *, int);
godot_error _put_partial_data(void *, const uint8_t *, int , int *);
int _get_available_bytes(const void *);

class StreamPeerUnix : public StreamPeerGDNative {
    GODOT_CLASS(StreamPeerUnix, StreamPeerGDNative)

	bool blocking;

public:
	enum SocketType {
		STREAM,
		DGRAM,
		SEQPACKET
	};

protected:
	int socketfd = -1;
	String path;

	godot_net_stream_peer interface = {
		{ 3, 1 },
		this,

		&_get_data,
		&_get_partial_data,
		&_put_data,
		&_put_partial_data,
		&_get_available_bytes,

		nullptr
	};

	struct sockaddr_un server_address;

	int get_socket_type(const SocketType type);


public:
	SocketType type;

	void _init();
	
	static void _register_methods();
	
	virtual godot_error get_data(uint8_t *p_buffer, int p_bytes);
	virtual godot_error get_partial_data(uint8_t *p_buffer, int p_bytes, int *r_received);
	virtual godot_error put_data(const uint8_t *p_data, int p_bytes);
	virtual godot_error put_partial_data(const uint8_t *p_data, int p_bytes, int *r_sent);
	virtual int get_available_bytes();

	int open(const String path);
	String get_path();
	bool is_open();
	void close();

	void set_blocking_mode(bool value);
	bool is_blocking_mode_enabled();

	void set_type(const int type);
	int get_type();

	~StreamPeerUnix();
	
};

#endif // STREAM_UNIX_H