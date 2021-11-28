#include "stream_peer_unix.h"

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

godot_error _get_data(void *user, uint8_t *p_buffer, int p_bytes) {
	return ((StreamPeerUnix *)user)->get_data(p_buffer, p_bytes);
}

godot_error _get_partial_data(void *user, uint8_t *p_buffer, int p_bytes, int *r_received) {
	return ((StreamPeerUnix *)user)->get_partial_data(p_buffer, p_bytes, r_received);
}

godot_error _put_data(void *user, const uint8_t *p_data, int p_bytes) {
	return ((StreamPeerUnix *)user)->put_data(p_data, p_bytes);
}

godot_error _put_partial_data(void *user, const uint8_t *p_data, int p_bytes, int *r_sent) {
	return ((StreamPeerUnix *)user)->put_partial_data(p_data, p_bytes, r_sent);
}

int _get_available_bytes(const void *user) {
	return ((StreamPeerUnix *)user)->get_available_bytes();
}


void StreamPeerUnix::_init() {
	net_api->godot_net_bind_stream_peer(_owner, &interface);
}

godot_error StreamPeerUnix::get_data(uint8_t *p_buffer, int p_bytes) {
	godot_error error = GODOT_OK;
	int recieved = 0;
	while (error == GODOT_OK && recieved < p_bytes) {
		int read;
		error = get_partial_data(p_buffer, p_bytes - recieved, &read);
		p_buffer += read;
		recieved += read;
	}
	return error;
}

godot_error StreamPeerUnix::get_partial_data(uint8_t *p_buffer, int p_bytes, int *r_received) {
	*r_received = 0;
	ERR_FAIL_COND_V(!is_open(), GODOT_ERR_UNCONFIGURED);
	int result = read(p_buffer, p_bytes);
	if (result < 0) {
		return GODOT_ERR_FILE_CANT_READ;
	} else if (result == 0) {
		close();
		return GODOT_ERR_FILE_EOF;
	}
	*r_received = result;
	return GODOT_OK;
}

godot_error StreamPeerUnix::put_data(const uint8_t *p_data, int p_bytes) {
	godot_error error = GODOT_OK;
	int sent = 0;
	while (error == GODOT_OK && sent < p_bytes) {
		int written;
		error = put_partial_data(p_data, p_bytes - sent, &written);
		p_data += written;
		sent += written;
	}
	return error;
}

godot_error StreamPeerUnix::put_partial_data(const uint8_t *p_data, int p_bytes, int *r_sent) {
	*r_sent = 0;
	ERR_FAIL_COND_V(!is_open(), GODOT_ERR_UNCONFIGURED);
	int result = write(p_data, p_bytes);
	if (result < 0) return GODOT_ERR_FILE_CANT_WRITE;
	*r_sent = result;
	return GODOT_OK;
}

int StreamPeerUnix::get_available_bytes() {
	ERR_FAIL_COND_V(!is_open(), -1);
	unsigned long available;
	ioctl(socketfd, FIONREAD, &available);
	return available;
}

int StreamPeerUnix::open(const String path) {
	ERR_FAIL_COND_V(is_open(), GODOT_ERR_ALREADY_IN_USE);
	socketfd = socket(AF_UNIX, get_socket_type(type), 0);
	if (socketfd < 0) {
		return GODOT_ERR_CANT_CREATE;
	}
	
	char *path_string = path.alloc_c_string();
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, path_string);
	if(::connect(socketfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
		socketfd = -1;
		return GODOT_ERR_CANT_CONNECT;
	}
	this->path = path;
	return GODOT_OK;
}

String StreamPeerUnix::get_path() {
	return path;
}

bool StreamPeerUnix::is_open() {
	return socketfd >= 0;
}

void StreamPeerUnix::close() {
	if (is_open()) {
		::close(socketfd);
		server_address = {};
		socketfd = -1;
		path = String();
	}
}

int StreamPeerUnix::read(uint8_t *p_buffer, const int p_bytes) {
	return recv(socketfd, p_buffer, p_bytes, 0);
}

int StreamPeerUnix::write(const uint8_t *p_data, const int p_bytes) {
	return send(socketfd, p_data, p_bytes, 0);
}

void StreamPeerUnix::set_type(const int type) {
	ERR_FAIL_COND(is_open());
	switch(type) {
		case SOCK_STREAM:
		case SOCK_DGRAM:
		case SOCK_SEQPACKET:
			this->type = static_cast<SocketType>(type);
			return;
	}
	ERR_PRINT("'type' can only take values from 0 to 2");
}

int StreamPeerUnix::get_type() {
	return type;
}

int StreamPeerUnix::get_socket_type(SocketType type) {
	switch(type) {
		case SOCK_STREAM: return ::SOCK_STREAM;
		case SOCK_DGRAM: return ::SOCK_DGRAM;
		case SOCK_SEQPACKET: return ::SOCK_SEQPACKET;
	}
}

StreamPeerUnix::~StreamPeerUnix() {
    close();
}

void StreamPeerUnix::_register_methods() {
	register_property<StreamPeerUnix, int>(
		"type",
        &StreamPeerUnix::set_type,    
        &StreamPeerUnix::get_type,    
        SocketType::SOCK_STREAM,
        GODOT_METHOD_RPC_MODE_DISABLED, 
        GODOT_PROPERTY_USAGE_DEFAULT, 
        GODOT_PROPERTY_HINT_ENUM, 
		"SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET"
	);
	
	register_method("open", &StreamPeerUnix::open);
	register_method("get_path", &StreamPeerUnix::get_path);
	register_method("is_open", &StreamPeerUnix::is_open);
	register_method("close", &StreamPeerUnix::close);
}