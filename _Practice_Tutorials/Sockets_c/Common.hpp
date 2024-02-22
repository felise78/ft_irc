#ifndef COMMON_HPP
#define COMMON_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include <iostream>
//#include <string>

// Some colors for the terminal
#define RED		"\033[0;31m"
#define GREEN	"\033[0;32m"
#define YELLOW	"\033[0;33m"
#define BLUE	"\033[0;34m"
#define MAGENTA	"\033[0;35m"
#define CYAN	"\033[0;36m"
#define RESET	"\033[0m"


#define SERVER_PORT		18000

#define LISTENQ			10
#define MAXLINE			4096
#define SA				struct sockaddr

void	printErrorAndExit(const char *fmt, ...);
char	*bin2hex(const unsigned char *input, size_t len);

#endif
