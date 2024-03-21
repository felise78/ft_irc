
NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

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
	Channel.cpp \
	Request.cpp \
	User.cpp \
	Join.cpp \
	Privmsg.cpp \
	Invite.cpp \
	Part.cpp \
	Topic.cpp \
	Kick.cpp \
	Quit.cpp \
	Nick.cpp

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
	$(MAKE) build -C Bot/GPT
	$(MAKE) run -C Bot/GPT
	$(MAKE) -C Bot
	mv Bot/bot .

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) bot
	$(MAKE) down -C Bot/GPT
# $(MAKE) clean -C Bot/GPT
# $(MAKE) fclean -C Bot

re: fclean all

.PHONY: all clean
