#include "../include/baseTetromino.hpp"

// Deveria colocar o destrutor aqui tbm, pra apagar os pointers pro shape e talz.

void Tetromino::rotateLeft(){
    size_t rows = shape.size();
    size_t cols = shape[0].size();

    // Cria a matriz rotacionada
    std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            rotated[j][rows - i - 1] = shape[i][j];
        }
    }   

    shape = rotated;

}

void Tetromino::rotateRight(){
    size_t rows = shape.size();
    size_t cols = shape[0].size();

    // Cria a matriz rotacionada com tamanho transposto
    std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows));

    // Preenche a matriz rotacionada
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            rotated[cols - j - 1][i] = shape[i][j];
        }
    }

    shape = rotated;

}

void Tetromino::moveLeft(){
    y--;
}

void Tetromino::moveRight(){
    y++;
}

void Tetromino::dropSlow(){
    x++;
}

// TODO (em tese a ideia seria de largar na posição final ? )
void Tetromino::dropFast(){
    x+=2;
}