#include "../include/board.hpp"

Board::Board(int W, int H, int cell_size) : WIDTH(W), HEIGHT(H), CELL_SIZE(cell_size), grid(HEIGHT, std::vector<int>(WIDTH, 0))
{
}

std::vector<std::vector<int>> Board::getGrid() const{
    return grid; 
}

void Board::render(sf::RenderWindow &window, const Tetromino &currentTetromino) const
{

    for (int row = 0; row < HEIGHT; ++row)
    {
        for (int col = 0; col < WIDTH; ++col)
        {

            // Coloca na posição que estamos interessado cada quadriculado

            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(col * CELL_SIZE, row * CELL_SIZE);

            // Information útile : Espace Vide, Tetromino actuel, Tetromino ancien

            if (grid[row][col] == 0)
            { // If there's no block here

                cell.setFillColor(sf::Color::Black);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::White);
                window.draw(cell);
            }

        }
    }

    // Renderizar o Tetromino atual
    const auto &shape = currentTetromino.getShape();
    int tetrominoX = currentTetromino.getX();
    int tetrominoY = currentTetromino.getY();

    sf::RectangleShape block(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    block.setFillColor(currentTetromino.getColor()); // Cor do Tetromino atual
    block.setOutlineThickness(1);
    block.setOutlineColor(sf::Color::Black); // Contorno para clareza

    for (size_t i = 0; i < shape.size(); ++i)
    {
        for (size_t j = 0; j < shape[i].size(); ++j)
        {
            if (shape[i][j] != 0)
            { // Se a célula faz parte do Tetromino
                block.setPosition((size_t)(normalizedX(tetrominoX + j)) * CELL_SIZE, (tetrominoY + i) * CELL_SIZE);
                window.draw(block);
            }
        }
    }
}

// Checks if the piece hit the bottom of the pit (next grid y position is bigger than
// the size of the board OR the next y position of any part of the shape is another block
// on the "pile" of fallen blocks)
bool Board::checkCollision(const Tetromino &currentTetromino)
{
    const auto &shape = currentTetromino.getShape();
    int tetrominoX = currentTetromino.getX();
    int tetrominoY = currentTetromino.getY();

    // Percorre a matriz de "shape" do Tetromino
    for (size_t i = 0; i < shape.size(); ++i)
    {
        for (size_t j = 0; j < shape[i].size(); ++j)
        {
            if (shape[i][j] != 0)
            { // Se a célula faz parte do Tetromino
                int gridX = normalizedX(tetrominoX + j);
                int gridY = tetrominoY + i;

                // Verifica se o bloco atingiu o fundo do tabuleiro
                if (gridY + 1 >= HEIGHT)
                {
                    return true; // Colisão com o fundo
                }

                // Verifica se o bloco atingiu outro bloco na "pilha"
                if (gridY +1 >= 0 && grid[gridY + 1][gridX] == -1)
                {
                    return true; // Colisão com a pilha de blocos
                }
            }
        }
    }

    return false;
}

int Board::normalizedX(int x) const
{

    x %= WIDTH;

    if (x < 0)
    {
        x += WIDTH;
    }

    return x;
}

// Integra o tetromino com o resto que já caiu
bool Board::placeTetromino(const Tetromino &currentTetromino, bool fallen)
{

    // Use os métodos virtuais de Tetromino aqui
    const auto &shape = currentTetromino.getShape();

    for (size_t i = 0; i < shape.size(); ++i)
    {
        for (size_t j = 0; j < shape[i].size(); ++j)
        {
            if (shape[i][j] != 0)
            { // Verifica se a célula faz parte do Tetromino
                int gridX = normalizedX(currentTetromino.getX() + j);
                int gridY = currentTetromino.getY() + i;

                grid[gridY][gridX] = (fallen)? -1 : 1; // Marca a célula 
            }
        }
    }

    return true; // Substitua pela lógica de posicionamento
}

void Board::clearFallingTetrominos()
{
    for (int row = 0; row < HEIGHT; ++row)
    {
        for (int col = 0; col < WIDTH; ++col)
        {
            // Limpa todas as células ocupadas por Tetrominos em queda (ID > 0)
            if (grid[row][col] > 0)
            {
                grid[row][col] = 0; // Reseta a célula para vazio
            }
        }
    }
}
