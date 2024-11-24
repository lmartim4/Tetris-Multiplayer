# Variáveis principais
SRC_DIR := src
INC_DIR := include
TARGET := tout

# Coleta todos os arquivos fonte em src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

# Configuração do compilador
CXX := g++
CXXFLAGS := -I$(INC_DIR) -Wall -O2
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Regra principal
all: $(TARGET)

# Como gerar o executável
$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Como compilar os arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar os arquivos compilados
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Executar o programa
run: $(TARGET)
	./$(TARGET)
