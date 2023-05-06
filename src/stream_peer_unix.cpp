#include "stream_peer_unix.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

Error StreamPeerUnix::_get_data(uint8_t *p_buffer, int32_t p_bytes, int32_t *r_received) {
  ERR_FAIL_COND_V(not is_open(), Error::ERR_UNCONFIGURED);
  ERR_FAIL_COND_V(p_bytes < 0, Error::ERR_INVALID_PARAMETER);

  Error error = Error::OK;
	int result = recv(socketfd, p_buffer, p_bytes, MSG_WAITALL | MSG_NOSIGNAL);
  *r_received = result;

	if (result < 0) {
    error = Error::ERR_FILE_CANT_READ;
    *r_received = 0;
	} else if (result == 0 && p_bytes > 0) {
    error = Error::ERR_FILE_EOF;
		close();
	} else if (result != p_bytes) {
    error = Error::ERR_INVALID_DATA;
	}
	return error;
}

Error StreamPeerUnix::_get_partial_data(uint8_t *p_buffer, int p_bytes, int *r_received) {
	*r_received = 0;
  ERR_FAIL_COND_V(not is_open(), Error::ERR_UNCONFIGURED);
  ERR_FAIL_COND_V(p_bytes < 0, Error::ERR_INVALID_PARAMETER);

  Error error = Error::OK;
	int result = recv(socketfd, p_buffer, p_bytes, MSG_NOSIGNAL);

	if (result < 0) {
    error = Error::ERR_FILE_CANT_READ;
		if (errno == EBADF || errno == EPIPE) close();
	} else if (result == 0 && p_bytes > 0) {
    error = Error::ERR_FILE_EOF;
		close();
	} else *r_received = result;

	return error;
}

Error StreamPeerUnix::_put_data(const uint8_t *p_data, int32_t p_bytes, int32_t *r_sent) {
  ERR_FAIL_COND_V(not is_open(), Error::ERR_UNCONFIGURED);

  Error error = Error::OK;
  while (error == Error::OK && *r_sent < p_bytes) {
		int32_t sent;
		error = _put_partial_data(p_data, p_bytes - sent, &sent);
		p_data += sent;
		*r_sent += sent;
	}

	return error;
}

Error StreamPeerUnix::_put_partial_data(const uint8_t *p_data, int32_t p_bytes, int32_t *r_sent) {
	*r_sent = 0;
  ERR_FAIL_COND_V(not is_open(), Error::ERR_UNCONFIGURED);

  Error error = Error::OK;
	int result = send(socketfd, p_data, p_bytes, MSG_NOSIGNAL);

	if (result < 0) {
    error = Error::ERR_FILE_CANT_WRITE;
		if (errno == EBADF || errno == EPIPE) close();
	} else if (result == 0 && p_bytes > 0) {
    error = Error::ERR_FILE_EOF;
		close();
	} else *r_sent = result;

	return error;
}

int32_t StreamPeerUnix::_get_available_bytes() const {
	ERR_FAIL_COND_V(!is_open(), -1);
	unsigned long available;
	ioctl(socketfd, FIONREAD, &available);
	return available;
}

int StreamPeerUnix::open(const String path) {
  ERR_FAIL_COND_V(is_open(), Error::ERR_ALREADY_IN_USE);

	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socketfd < 0) {
    return Error::ERR_CANT_CREATE;
	}
	
	const char *path_string = path.utf8().get_data();
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, path_string);

	if(::connect(socketfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
		socketfd = -1;
    return Error::ERR_CANT_OPEN;
	}

	if (!blocking) fcntl(socketfd, F_SETFL, O_NONBLOCK);

	this->path = path;

  return Error::OK;
}

String StreamPeerUnix::get_path() const {
	return path;
}

bool StreamPeerUnix::is_open() const {
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

void StreamPeerUnix::set_blocking_mode(bool value) {
	ERR_FAIL_COND(is_open());
	blocking = value;
}

bool StreamPeerUnix::is_blocking_mode_enabled() const {
	return blocking;
}

StreamPeerUnix::~StreamPeerUnix() {
    close();
}

void StreamPeerUnix::_bind_methods() {
  ClassDB::bind_method(D_METHOD("open"), &StreamPeerUnix::open);
  ClassDB::bind_method(D_METHOD("get_path"), &StreamPeerUnix::get_path);
  ClassDB::bind_method(D_METHOD("is_open"), &StreamPeerUnix::is_open);
  ClassDB::bind_method(D_METHOD("close"), &StreamPeerUnix::close);

  ClassDB::bind_method(D_METHOD("set_blocking_mode"),
                       &StreamPeerUnix::set_blocking_mode);
  ClassDB::bind_method(D_METHOD("is_blocking_mode_enabled"),
                       &StreamPeerUnix::is_blocking_mode_enabled);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "blocking_mode"),
               "set_blocking_mode", "is_blocking_mode_enabled");
}
