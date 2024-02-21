
CXX = g++
CXXFLAGS = -std=c++98 -g

COMMON_OBJS = Common.o

#NAME = tcpc
#SRC = TcpClient.cpp
#OBJ = $(SRC:.cpp=.o)

all: $(COMMON_OBJS) tcpClient tcpServer clean

Common.o: Common.cpp Common.hpp
	$(CXX) $(CXXFLAGS) -c Common.cpp

tcpClient: TcpClient.cpp $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o tcpClient TcpClient.cpp $(COMMON_OBJS)

tcpServer: TcpServer.cpp $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o tcpServer TcpServer.cpp $(COMMON_OBJS)

clean:
	rm -f *.o

fclean: clean
	rm -f tcpClient tcpServer

re: fclean all

.PHONY: clean fclean all re
