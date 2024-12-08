#include "game/gameManager.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm> // for std::max, std::min

#define P(x) ((280 * (x * x * x) - 1470 * (x * x) + 2630 * (x) - 1320) / 3)
#define LEVEL_UP 10
#define DOWN_FASTER 2 // Define this appropriately if not defined elsewhere

GameManager::GameManager(ServerManager &serverManager)
    : board(serverManager),
      isRunning(false),
      score(0),
      level(0),
      nLinesClearedThisLevel(0),
      levelUpGravityTimeMs(100),
      threadActive(false)
{
    // Initialize gravityTimeMs to a default, for example 800ms:
    gravityTimeMs = 600;
}

int GameManager::lines2Points(int nLines)
{
    return (nLines > 0) ? P(std::min(nLines, 4)) * (level + 1) : 0;
}

void GameManager::startGameLoop()
{
    if (isRunning.load())
    {
        std::cerr << "Game is already running!" << std::endl;
        return;
    }

    isRunning.store(true);
    threadActive.store(true);
    // Spawn a new thread that runs runGameLoop()
    gameThread = std::thread(&GameManager::runGameLoop, this);
    std::cout << "Game Started!";
}

void GameManager::stopGameLoop()
{
    // Signal the loop to stop
    isRunning.store(false);

    // Wait for thread to finish if joinable
    if (gameThread.joinable())
    {
        gameThread.join();
    }
}

bool GameManager::isGameRunning() const
{
    return isRunning.load();
}

bool GameManager::isThreadRunning() const
{
    return threadActive.load();
}

void GameManager::spawnTetromino()
{
    currentTetromino = TetrominoFactory::createTetromino();
}

void GameManager::runGameLoop()
{
    srand(time(NULL));
    spawnTetromino();

    // Initialize lastGravityTick
    lastGravityTick = std::chrono::steady_clock::now();

    while (isRunning)
    {
        TetrisAction action;
        while (actionQueue.pop(action))
            handleInput(action);
    
        update();
        board.broadcastBoardState();
        // board.printStatus();
        //  Sleep a bit to avoid busy-waiting and allow input handling at ~60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    // Loop ended
    threadActive.store(false);
}

void GameManager::update()
{
    std::lock_guard<std::mutex> lock(gameStateMutex);

    if (board.reachedTop())
    {
        std::cout << "(GAME OVER) !!!" << std::endl;
        std::cout << "(Final score) : " << score << " !!!" << std::endl;
        std::cout << "You played till level: " << level << std::endl;

        board.clear();
        // Exit game loop by setting isRunning to false
        isRunning.store(false);
        return; // Return here to stop processing
    }

    // Clear old tetrominos
    board.clearFallingTetrominos();

    TetrisAction lastMovereceived = this->lastM;

    // Check if it's time to apply gravity
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastGravityTick).count() > gravityTimeMs)
    {
        currentTetromino->dropGravity();
        lastGravityTick = now;
        lastMovereceived = TetrisAction::EMPTY;
    }
    else
    {
        this->lastM = TetrisAction::EMPTY;
    }

    // Check collision after gravity or moves
    if (board.checkCollision(*currentTetromino, lastMovereceived))
    {
        if (lastMovereceived == TetrisAction::DROP_FASTER || currentTetromino->getGravity())
        {
            // Place and lock tetromino
            std::cout << "Last move received = " << TetrisActionToString(lastMovereceived) << " e gravidade = " << currentTetromino->getGravity() << std::endl;
            board.placeTetromino(*currentTetromino, true);

            // GRAVIDADE TEM Q SAIR DO TETROMINO, é uyma propriedade GLOBAL agr
            currentTetromino->gravity = false;

            // std::cout << "Encaixei" << std::endl;
            currentTetromino.reset();

            nLinesClearedThisLevel += board.clearLines();
            score += lines2Points(nLinesClearedThisLevel);

            if (nLinesClearedThisLevel >= LEVEL_UP)
            {
                level++;
                // Clear all fixed blocks
                board.clearFalledTetrominos();

                gravityTimeMs = std::max(gravityTimeMs - levelUpGravityTimeMs, minTimeMs);
                lastGravityTick = std::chrono::steady_clock::now();
                nLinesClearedThisLevel = 0;
                std::cout << "(Current score) : " << score << " !!!" << std::endl;
                std::cout << "(LEVEL UP) !!!" << std::endl;
            }

            // Spawn a new tetromino
            spawnTetromino();
        }
        else
        {
            // Invalid move, revert
            board.placeTetromino(*currentTetromino, false);
            currentTetromino->gravity = false;
        }
    }
    else
    {
        // Valid move, place without locking
        board.placeTetromino(*currentTetromino, false);
        currentTetromino->gravity = false;
    }
}

void GameManager::handleInput(TetrisAction action)
{
    std::lock_guard<std::mutex> lock(gameStateMutex);
    lastM = action;
}
