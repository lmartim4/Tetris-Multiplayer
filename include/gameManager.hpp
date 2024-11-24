#ifndef GAME_MANAGER
#define GAME_MANAGER

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "tetrominoFactory.hpp" // Assuming this contains Tetromino and related classes
#include "board.hpp"            // Assuming Board class for grid rendering
#include <iostream>

class GameManager
{
private:
    Board board;
    std::unique_ptr<Tetromino> currentTetromino;
    bool isRunning;
    sf::RenderWindow window;
    sf::Clock gameClock;

    // Grid constants
    static const int CELL_SIZE = 40; // Agora é static
    static const int HEIGHT = 20, WIDTH = 10;
    static const int H_RES = CELL_SIZE * HEIGHT;
    static const int W_RES = CELL_SIZE * WIDTH;
    
    // Tetromino constants (awful for the moment, due to the logic of size) 
    static const int NUM_TETRO = 7;
    static const int NUM_COLORS = 6;
    static const char types[NUM_TETRO]; // Declarar o array aqui
    static const std::vector<sf::Color> GameManager::tetromino_colors;

    // Game constants
    static const int gravityTimeMs = 700;
    sf::Time gravityTime;

public:
    GameManager();

    void runGameLoop();

    void renderTetromino();

    void spawnTetromino();

    /* ---------- Game Loop functions --------- */
    
    void handleInput();

    void update();

    void render();

    /* ---------- ---------- ---------- --------- */
};


#endif