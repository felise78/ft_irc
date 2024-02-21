#include "Common.hpp"

int	main(int argc, char **argv) {

	int					listen_fd;
	int					conn_fd;
	int					n;
	int					return_value;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE + 1];
	uint8_t				recvline[MAXLINE + 1];

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0) {
		printErrorAndExit("Error creating socket!");
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT); // server port to listen on, must be > 1024 if running as a non-root user

	return_value = bind(listen_fd, (SA *)&servaddr, sizeof(servaddr));
	if (return_value < 0) {
		printErrorAndExit("bind error");
	}

	return_value = listen(listen_fd, LISTENQ);
	if (return_value < 0) {
		printErrorAndExit("listen error");
	}

	std::cout << MAGENTA << "\tListening on port " << SERVER_PORT << RESET << std::endl;

	while (1) {

		struct sockaddr_in	client_addr;
		socklen_t			client_addr_len = sizeof(client_addr);
		char				client_addr_str[INET_ADDRSTRLEN];

		std::cout << CYAN << "Waiting for a connection on port: " << SERVER_PORT << RESET << std::endl;
		// accept() blocks until a client connects to the server
		// ..by setting the last two arguments to NULL, we don't care about who is connecting to us.
		//conn_fd = accept(listen_fd, (SA *) NULL, NULL);
		conn_fd = accept(listen_fd, (SA *)&client_addr, &client_addr_len);
		if (conn_fd < 0) {
			printErrorAndExit("accept error");
		}

		// inet_ntop converts the client's IP address to a string (network to presentation)
		inet_ntop(AF_INET, &client_addr, client_addr_str, INET_ADDRSTRLEN);
		std::cout << MAGENTA << "\tConnection accepted from [" << client_addr_str << "]:[" << ntohs(client_addr.sin_port) << "]" << RESET << std::endl;

		memset(recvline, 0, MAXLINE);

		// read() client's message from the socket
		while ( (n = read(conn_fd, recvline, MAXLINE - 1)) > 0) {

			//fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);
			std::cout << "recvline: " << recvline << std::endl;

			// detect end of HTTP request.. not robust..
			if (recvline[n - 1] == '\n') {
				break ;
			}

			//printf("%s", recvline);
			memset(recvline, 0, MAXLINE);
		}

		if (n < 0) {
			printErrorAndExit("read error");
		}

		// Send the response to the client
		snprintf(buff, sizeof(buff), "HTTP/1.1 200 OK\r\n\r\nHello");

		return_value = write(conn_fd, buff, strlen(buff));
		if (return_value != strlen(buff)) {
			printErrorAndExit("write error");
		}
		close(conn_fd);
	}

}
