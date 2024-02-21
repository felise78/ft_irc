/*
**
** In the example bellows is the simple server setting that is
** a Synchrounous I/O Multiplexing Server, which provides a simple
** blocking approach to handling multiple clients.
** Meaning that the server can only handle one client at a time.
** This is not a good approach for a server.
**
** The usage of `select()` is added to the main loop, which will
** monitor the server socket for incoming connections.
** Asynchronous I/O Multiplexing Server is a better approach.
** A later example of using `select()` will provide a better approach
** to monitor multiple sockets/connections at the same time, and
** handle them as they become available. 
** This will still use a blocking approach, but will allow the server 
** to handle multiple clients. This will be done without threading.
*/

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT		8080
#define SERVER_BACKLOG	10
#define SOCKET_ERROR	-1
#define BUFSIZE			10240
#define PATH_MAX		1024	

int		setup_server(int port, int backlog);
int		accept_new_connection(int server_fd);
void	*handle_connection(int client_fd);
int		check(int exp, const char *msg);

int	main(int argc, char **agrv) {

	// Initializing the server with `socket()` (returns server_fd) and `bind()` to a port
	int		server_fd = setup_server(SERVER_PORT, SERVER_BACKLOG);

	// we need two fd_set variables since it is modified by select()
	// current_sockets will always contain the original sockets to monitor
	fd_set	master_sockets;
	fd_set	ready_sockets;

	int		select_return = 0;
	// initializing current fd_set
	FD_ZERO(&master_sockets);
	// adding server_fd to the current fd_set
	FD_SET(server_fd, &master_sockets);

	std::cout << "FD_SETSIZE: " << FD_SETSIZE << std::endl;

	while (true) {

		// copy the current fd_set to the ready fd_set
		ready_sockets = master_sockets;


		select_return = select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL);
		if (select_return < 0) {
			perror("select error");
			exit(EXIT_FAILURE);
		}

		// loop through all the possible file descriptors
		for (int i = 0; i < FD_SETSIZE; i++) {

			// check to see if the current file descriptor is in the ready fd_set
			if (FD_ISSET(i, &ready_sockets)) {

				// check to see if the current file descriptor is the server_fd
				if (i == server_fd) {

					// accept new connection
					int		client_fd = accept_new_connection(server_fd);

					// add the new connection to the current fd_set
					FD_SET(client_fd, &master_sockets);
				}
				else {

					// handle the current connection
					handle_connection(i);

					// remove the current connection from the current fd_set
					FD_CLR(i, &master_sockets);
				}
			}
		}

		std::cout << "Waiting for connection..." << std::endl;

		int		client_fd = accept_new_connection(server_fd);

		handle_connection(client_fd);
	}

	return 0;
}

int	setup_server(int port, int backlog) {

	int					server_fd;
	int					client_fd;
	struct sockaddr_in	server_addr;
	int					addr_size = sizeof(server_addr);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	check(server_fd, "Failed to create socket");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	int		bind_result = bind(server_fd, (struct sockaddr *)&server_addr, addr_size);
	check(bind_result, "Bind Failed !");

	int		listen_result = listen(server_fd, backlog);
	check(listen_result, "Listen Failed !");

	return (server_fd);
}

int	accept_new_connection(int server_fd) {

	int					client_fd;
	struct	sockaddr_in client_addr;
	int					addr_size = sizeof(client_addr);

	client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);
	check(client_fd, "Accept Failed !");

	std::cout << "Connection accepted from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

	return (client_fd);
}

/*
** The most of the server's heavy lifting is done here.
*/
void	*handle_connection(int client_fd) {

	char	buffer[BUFSIZE] = {0};
	char	actual_path[PATH_MAX + 1];
	int		bytes_read;
	int		msg_size = 0;

	//while ((bytes_read = recv(client_fd, buffer, BUFSIZE, 0)) > 0) {
	while ((bytes_read = read(client_fd, buffer+msg_size, BUFSIZE)) > 0) {

		msg_size += bytes_read;
		if (msg_size >= BUFSIZE || buffer[msg_size - 1] == '\n') {
			break ;
		}
	}
	check(bytes_read, "recv error");
	buffer[msg_size - 1] = '\0';

	std::cout << "Received: " << buffer << std::endl;
	fflush(stdout);

	if (realpath(buffer, actual_path) == NULL) {
		std::cout << "ERROR. Invalid path: " << buffer << std::endl;
		close(client_fd);
		return (NULL);
	}

	// read file and send its contents to the client
	FILE	*fp = fopen(actual_path, "r");
	if (fp == NULL) {
		std::cout << "ERROR(open): " << actual_path << std::endl;
		close(client_fd);
		return (NULL);
	}

	// read file contents and send them to the client
	// for better security, we would have to limit the the client to certain files
	while ((bytes_read = fread(buffer, 1, BUFSIZE, fp)) > 0) {
		
		write(client_fd, buffer, bytes_read);
		//send(client_fd, buffer, bytes_read, 0);
	}
	close(client_fd);
	fclose(fp);
	std::cout << "Closing connection" << std::endl;

	return (NULL);
}

int	check(int exp, const char *msg) {

	if (exp == SOCKET_ERROR) {
		perror(msg);
		exit(1);
	}

	return (exp);
}

