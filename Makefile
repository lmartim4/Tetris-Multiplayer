# Variables
SRC_FILES := main.cpp # List of source files, update as needed
OBJ_FILES := $(SRC_FILES:.cpp=.o)     # Convert source file names to object file names
TARGET := tout                        # Name of the final executable

# Default target
all: $(TARGET)

# Build the target executable by linking object files
$(TARGET): $(OBJ_FILES)
	g++ -o $(TARGET) $(OBJ_FILES)

# Compile .cpp files to .o files (generic rule)
%.o: %.cpp
	g++ -c $< -o $@

# Run the executable
run:
	./$(TARGET)

# Clean up the object files and the executable
clean:
	rm -f $(OBJ_FILES) $(TARGET)