
NAME = bot

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS = mainBot.cpp \
	IrcBot.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

gpt:
	$(MAKE) build -C GPT
	sleep 5
	$(MAKE) run -C GPT

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	make clean

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
# $(MAKE) down -C GPT
# $(MAKE) clean -C GPT

re: fclean all

.PHONY: all clean
