
NAME = bot

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS = mainBot.cpp \
	IrcBot.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	make clean

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean
