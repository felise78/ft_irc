
NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g

VPATH = Server \
		Commands \
		Request \
		Channel \
		User

SRCS = main.cpp \
	Server.cpp \
	ServerManager.cpp \
	server_utils.cpp \
	UserRequestParsing.cpp \
	CommandHandler.cpp \
	ModeHandler.cpp \
	UserResponse.cpp \
	Channel.cpp \
	Request.cpp \
	User.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
# cleaning the objects right after make
	make clean

valgrind: $(NAME)
	clear
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=all ./$(NAME)

bot:
	$(MAKE) -C Bot
	mv Bot/bot .

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) bot

re: fclean all

.PHONY: all clean
