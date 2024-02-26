
NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g

VPATH = Server \
		Commands \
		Request \
		User

SRCS = main.cpp \
	Server.cpp \
	ServerManager.cpp \
	UserRequestParsing.cpp \
	UserResponse.cpp
#	CommandHandler.cpp \

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
# cleaning the objects right after make
	make clean

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean
