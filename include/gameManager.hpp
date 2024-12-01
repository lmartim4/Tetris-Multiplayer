#ifndef GAME_MANAGER
#define GAME_MANAGER

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "tetrominoFactory.hpp" 
#include "board.hpp"            
#include <iostream>
#include <ctime>

class GameManager
{
private:
    Board board;
    std::unique_ptr<Tetromino> currentTetromino;
    bool isRunning;
    sf::Clock gameClock;
    
    // Game constants
    static const int gravityTimeMs = 700;
    sf::Time gravityTime;

public:
    GameManager();

    void runGameLoop();

    void spawnTetromino();

    void update();

};


#endif