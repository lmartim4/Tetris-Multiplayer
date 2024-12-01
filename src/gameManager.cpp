#include "../include/gameManager.hpp"

const int GameManager::gravityTimeMs;

GameManager::GameManager()
    : board(), 
      isRunning(false),
      gravityTime(sf::milliseconds(GameManager::gravityTimeMs))
{
}

void GameManager::runGameLoop()
{
    this->isRunning = true;

    // Init timer
    srand((int)time(0));
    gameClock.restart();
    spawnTetromino();

    while (board.windowIsOpen())
    {
        board.handleInput(*currentTetromino);
        update();
        board.render();
    }
}

void GameManager::spawnTetromino()
{
    currentTetromino = TetrominoFactory::createTetromino();
}

void GameManager::update()
{

    // Apagar os tetrominos anteriores
    board.clearFallingTetrominos();

    // Gravidade -> Checar mais tarde pra condições de se já ta embaixo
    if (gameClock.getElapsedTime() > gravityTime)
    {
        currentTetromino->dropGravity();
        gameClock.restart();
    }

    int lastMove = currentTetromino->getLastMove();

    // Só deve dar updateStates depois de checar contra a board
    if (board.checkCollision(*currentTetromino) )
    {
        if(lastMove == DOWN_FASTER || currentTetromino->getGravity()){

            // Place tetromino on the board
            board.placeTetromino(*currentTetromino, true); 
            currentTetromino->updateStates();
            currentTetromino.reset();
            
            board.clearLines();
            
            // Spawn a new tetromino
            spawnTetromino(); 

        }else{ // Just invalid, just clean it's action

            board.placeTetromino(*currentTetromino, false); 
            currentTetromino->updateStates();
        }
    }else{ // Atualiza o Tetromino ativo no jogo (movimenta e reposiciona)

        board.placeTetromino(*currentTetromino, false);
        currentTetromino->updateStates();
    }
}