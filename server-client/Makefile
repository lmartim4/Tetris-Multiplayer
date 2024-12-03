# Define the compiler and flags
CXX = g++
CXXFLAGS = -c
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lenet

# Define targets for client and server
CLIENT_TARGET = client-app
SERVER_TARGET = server-app

# Define object files for client and server
CLIENT_OBJ = TetrisClient.o ClientManager.o NetworkManager.o
SERVER_OBJ = TetrisServer.o ServerManager.o NetworkManager.o

# Default target: build both client and server
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# Rule for building the client executable
$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CXX) $(CLIENT_OBJ) -o $(CLIENT_TARGET) $(LDFLAGS)

# Rule for building the server executable
$(SERVER_TARGET): $(SERVER_OBJ)
	$(CXX) $(SERVER_OBJ) -o $(SERVER_TARGET) $(LDFLAGS)

# Rule for compiling client.cpp and other client source files
TetrisClient.o: TetrisClient.cpp ClientManager.hpp NetworkManager.hpp TetrisActions.hpp
	$(CXX) $(CXXFLAGS) TetrisClient.cpp

ClientManager.o: ClientManager.cpp ClientManager.hpp NetworkManager.hpp TetrisActions.hpp
	$(CXX) $(CXXFLAGS) ClientManager.cpp

# Rule for compiling server.cpp and other server source files
TetrisServer.o: TetrisServer.cpp ServerManager.hpp NetworkManager.hpp TetrisActions.hpp
	$(CXX) $(CXXFLAGS) TetrisServer.cpp

ServerManager.o: ServerManager.cpp ServerManager.hpp NetworkManager.hpp TetrisActions.hpp
	$(CXX) $(CXXFLAGS) ServerManager.cpp

# Rule for compiling shared NetworkManager.cpp
NetworkManager.o: NetworkManager.cpp NetworkManager.hpp
	$(CXX) $(CXXFLAGS) NetworkManager.cpp

# Run the client
run-client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

# Run the server
run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Clean up the build
clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) $(CLIENT_TARGET) $(SERVER_TARGET)