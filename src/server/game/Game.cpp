#include "Game.hpp"
#include "TetrominoFactory.hpp"

#define LEVEL_UP 10

int Game::instanceCount = 0;

void Game::startGameLoop()
{
    static int instanceCount = 0;
    if (gameState != GameState::WAITING_PLAYERS)
    {
        std::cerr << "Cannot start a game that is not currently waiting for players" << std::endl;
        return;
    }

    gameState = GameState::STARTING;
    console_log("Starting Game (" + std::to_string(instanceCount++) + ")...");

    gameThread = std::thread(&Game::loop, this);
}

void Game::endGameLoop()
{
    if (gameState != GameState::RUNNNING)
    {
        console_log("Cannot end a game that is not currently running");
        return;
    }

    gameState = GameState::ENDING;

    if (gameThread.joinable())
        gameThread.join();

    if (gameState == GameState::ENDED)
    {
        console_log("Successfully ended gameLoop");
    }
    else
    {
        throw std::runtime_error("Failed to end game Loop");
    }
}

int Game::calculateLinesToPoints(int nLines, int level)
{
    int x = std::min(nLines, 4);
    int P_x = ((280 * (x * x * x) - 1470 * (x * x) + 2630 * (x)-1320) / 3);

    return (nLines > 0) ? P_x * (level + 1) : 0;
}

void Game::handleInput(Player *player, TetrisAction action)
{
    player->actions.push(action);
}

void Game::loop()
{
    gameState = GameState::RUNNNING;

    spawnTetromino();
    levelData.updateGravityTick();

    while (gameState != GameState::ENDING)
    {
        TetrisAction action;

        for (Player *pl : players)
            while (pl->actions.pop(action))
                updateGame(action);
        
        updateGame(TetrisAction::EMPTY);
        sendBoardUpdates();
        // board.printStatus();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    gameState = GameState::ENDED;
}

void Game::updateGame(TetrisAction lastAction)
{
    // Check for game-over condition
    if (board.reachedTop())
    {
        std::cout << "(GAME OVER) !!!" << std::endl;
        std::cout << "(Final score): " << levelData.getScore() << std::endl;
        std::cout << "You played till level: " << levelData.getLevel() << std::endl;

        board.clear();
        gameState = GameState::ENDING;
        return;
    }

    // Clear old tetrominos before processing the new action
    board.clearFallingTetrominos();

    bool gravityApplied = levelData.gravityIntervalPassed();

    // Handle tetromino collision
    if (board.checkCollision(*currentTetromino, lastAction, gravityApplied))
    {
        // Place the tetromino if it has fallen or the action was DROP_FASTER
        if (lastAction == TetrisAction::DROP_FASTER || gravityApplied)
        {
            lockTetromino();
            clearFullLines();

            if (levelData.attemptLevelUp(LEVEL_UP))
            {
                board.clearFalledTetrominos();
                std::cout << "(Current score): " << levelData.getScore() << std::endl;
                std::cout << "(LEVEL UP) !!!" << std::endl;
            }

            spawnTetromino();
        }
        else
        {
            // Invalid move, revert the tetromino state
            board.placeTetromino(*currentTetromino, false);
        }
    }
    else
    {
        // Valid move, place tetromino without locking
        board.placeTetromino(*currentTetromino, false);
    }
}

// Locks the current tetromino into place and prepares for new actions
void Game::lockTetromino()
{
    board.placeTetromino(*currentTetromino, true);
    currentTetromino.reset();
}

void Game::clearFullLines()
{
    levelData.addClearedLines(board.clearLines());
    levelData.addScore(calculateLinesToPoints(levelData.getClearedLinesThisLevel(), levelData.getLevel()));
}

Game::Game(PacketSender *sender) : Debuggable("Game"), packetSender(sender), this_instance(instanceCount++), board(16, 10)
{
    console_log("Initializing Game (" + std::to_string(this_instance) + ")...");
    gameState = GameState::WAITING_PLAYERS;
}

Game::~Game()
{
    console_log("Destroying Game (" + std::to_string(this_instance) + ")...");
}

void Game::addPlayer(Player *player)
{
    if (gameState != GameState::WAITING_PLAYERS)
    {
        console_log("Cannot add player (not waiting for players)");
        return;
    }

    players.emplace_back(player);
}

void Game::spawnTetromino()
{
    currentTetromino = TetrominoFactory::createTetromino();
}

void Game::sendBoardUpdates()
{
    if (currentTetromino->shouldBroadcastState())
    {
        console_log("Sending board!");
        nlohmann::json boardJson = board.constructBoardJsonToBroadcast();
        sendBoardState(boardJson);
    }
}

void Game::sendBoardState(const nlohmann::json &boardJson) const
{
    packetSender->sendPacket(Packet(PacketType::GAME_SCREEN, boardJson, nullptr));
}