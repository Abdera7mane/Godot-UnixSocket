#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#define SOCKET_PATH "/tmp/godot_unix_test.sock"

int main() {
	struct sockaddr_un server;
	int socketfd;
	
	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socketfd < 0) {
		perror("SERVER: Could not create socket");
		return 1;
	}

	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, SOCKET_PATH);
	unlink(server.sun_path);
	if (bind(socketfd, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) < 0) {
		perror("SERVER: Could not bind socket");
		return 1;
	}

	if (listen(socketfd, 2) != 0) {
		perror("SERVER: Could not listen to socket");
		return 1;
	}

	printf("SERVER: Socket listening on: %s\n", SOCKET_PATH);
	printf("SERVER: Wating for connection\n");
	int client_socketfd;
	client_socketfd = accept(socketfd, nullptr, nullptr);

	if (client_socketfd < 0) {
		perror("SERVER: Failed accepting socket connection");
		return 1;
	}

	int buffer_size = 256;
	char buffer[buffer_size];
	bzero(&buffer, buffer_size - 1);
	
	int bytes = recv(client_socketfd, buffer, buffer_size - 1, 0);

	if (bytes > 0) {
		printf("SERVER: Message recieved: %s\n", buffer);
		bzero(&buffer, buffer_size - 1);
		
		if (send(client_socketfd, "Hello Client", 12, 0) < 0) {
			perror("SERVER: ERROR echoing back to socket");
			return 1;
		}
		printf("SERVER: Message echoed back\n");
	}
	
	else {
		perror("SERVER: ERROR reading from socket");
		return 1;
	}
	
	close(client_socketfd);
	close(socketfd);
	unlink(server.sun_path);
	printf("SERVER: Disocnnected\n");
}