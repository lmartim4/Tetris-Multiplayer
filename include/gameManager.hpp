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
    int score;
    int level;
    int nLinesClearedThisLevel;
    const int minTimeMs = 400;
    sf::Clock gameClock;
    
    // Game constants
    int gravityTimeMs;
    int levelUpGravityTimeMs;
    sf::Time gravityTime;

public:
    GameManager();

    int lines2Points(int nLines); 
    void runGameLoop();

    void spawnTetromino();

    void update();

};


#endif