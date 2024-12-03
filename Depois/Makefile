# Define the compiler and flags
CXX = g++
CXXFLAGS = -c
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lenet

# Define targets for client and server
CLIENT_TARGET = client-app
SERVER_TARGET = server-app

# Define object files
CLIENT_OBJ = client.o
SERVER_OBJ = server.o

# Default target: build both client and server
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# Rule for building the client executable
$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CXX) $(CLIENT_OBJ) -o $(CLIENT_TARGET) $(LDFLAGS)

# Rule for building the server executable
$(SERVER_TARGET): $(SERVER_OBJ)
	$(CXX) $(SERVER_OBJ) -o $(SERVER_TARGET) $(LDFLAGS)

# Rule for compiling client.cpp
$(CLIENT_OBJ): client.cpp
	$(CXX) $(CXXFLAGS) client.cpp

# Rule for compiling server.cpp
$(SERVER_OBJ): server.cpp
	$(CXX) $(CXXFLAGS) server.cpp

# Run the client
run-client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

# Run the server
run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Clean up the build
clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) $(CLIENT_TARGET) $(SERVER_TARGET)
