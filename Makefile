
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
	UserRequestParsing.cpp \
	CommandHandler.cpp \
	UserResponse.cpp \
	Channel.cpp \
	User.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
# cleaning the objects right after make
	make clean

bot:
	$(MAKE) -C Bot
	mv Bot/bot .

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) bot

re: fclean all

.PHONY: all clean
