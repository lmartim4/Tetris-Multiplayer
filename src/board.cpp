#include "../include/board.hpp"

Board::Board(int W, int H, int cell_size):
WIDTH(W), HEIGHT(H), CELL_SIZE(cell_size), grid(HEIGHT, std::vector<int>(WIDTH, 0))
{

}

void Board::render(sf::RenderWindow& window) const {

    for (int row = 0; row < HEIGHT; ++row) {
        for (int col = 0; col < WIDTH; ++col) {
            
            // Coloca na posição que estamos interessado cada quadriculado

            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE,CELL_SIZE));
            cell.setPosition(col * CELL_SIZE, row * CELL_SIZE);


            // Information útile : Espace Vide, Tetromino actuel, Tetromino ancien 

            if (grid[row][col] == 0) {  // If there's no block here
                
                cell.setFillColor(sf::Color::Black);  
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::White);
            }else{ // For now, no colision with the ground
                cell.setFillColor(sf::Color::Blue);
            }

            window.draw(cell);
        }
    }
    
}

bool Board::checkCollision(const Tetromino &currentTetromino) {
    // Use os métodos virtuais de Tetromino aqui
    return false; // Substitua pela lógica de colisão
}

// Integra o tetromino com o resto que já caiu 
bool Board::placeTetromino(const Tetromino &currentTetromino, int x, int y) {

    // Use os métodos virtuais de Tetromino aqui
    const auto &shape = currentTetromino.getShape();
    
    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j] != 0) { // Verifica se a célula faz parte do Tetromino
                int gridX = x + j;
                int gridY = y + i;

                if (gridX >= 0 && gridX < WIDTH && gridY >= 0 && gridY < HEIGHT) {
                    grid[gridY][gridX] = 1; // Marca a célula como ocupada
                }
            }
        }
    }

    return true; // Substitua pela lógica de posicionamento
}
