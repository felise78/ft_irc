/*
** Credit for this tutorial to:
**
** Skrew Everything at https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
** and
** Eric O Meehan at https://www.youtube.com/watch?v=YwHErWJIh6Y&t=11s
**
*/

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080 // Entry point for client

int	main(int argc, char **argv) {

	int 	server_fd;
	int 	new_socket;
	long	valread;
	struct sockaddr_in address;
	int 	addrlen = sizeof(address);

	const char 	*hello = "Hello from server";

	// 1. Creating socket, getting its file descriptor 
	server_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP
	if (server_fd == 0) {
		
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	// 2. Identifying socket 

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	memset(address.sin_zero, '\0', sizeof address.sin_zero);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	// 3. Waiting for client to connect, maximum 10 clients allowed
	if (listen(server_fd, 10) < 0) {
		
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	while (1) {

		printf("\n+++++++ Waiting for new connection ++++++++\n\n");

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
			
			perror("In accept");
			exit(EXIT_FAILURE);
		}

		// 4. Send and receive messages

		char buffer[30000] = {0};
		valread = read(new_socket, buffer, 30000);
		printf("%s\n", buffer);
		write(new_socket, hello, strlen(hello));
		printf("------------------Hello message sent-------------------\n");

		// 5. Close the socket
		close(new_socket);
	}

	return 0;
}

/*
** compiling this file `g++ tcpSocketServerSide.cpp -o serverSide`
** and running it `./serverSide` will start the server
** and it will wait for a client to connect to it..
**
** to connect to the server, we need to run the client side
** `./clientSide` (see another file in this directory)
** compiling as `g++ tcpSocketClientSide.cpp -o clientSide`
*/
