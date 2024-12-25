#include "Game.hpp"
#include "TetrominoFactory.hpp"

#define LEVEL_UP_LINES 10

int Game::instanceCount = 0;

Game::Game(PacketSender *sender) : Debuggable("Game"), packetSender(sender), this_instance(instanceCount++), board(16, 10)
{
    console_log("Initializing Game (" + std::to_string(this_instance) + ")...");
    gameState = WAITING_PLAYERS;
}

Game::~Game()
{
    console_log("Destroying Game (" + std::to_string(this_instance) + ")...");
}

void Game::addPlayer(Player *player)
{
    if (gameState != WAITING_PLAYERS)
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

void Game::startGameLoop()
{
    static int instanceCount = 0;

    if (gameState != WAITING_PLAYERS)
    {
        std::cerr << "Cannot start a game that is not currently waiting for players" << std::endl;
        return;
    }

    gameState = STARTING;
    console_log("Starting Game (" + std::to_string(instanceCount++) + ")...");

    gameThread = std::thread(&Game::loop, this);
}

void Game::endGameLoop()
{
    if (gameState != RUNNNING)
    {
        console_log("Cannot end a game that is not currently running");
        return;
    }

    gameState = ENDING;

    if (gameThread.joinable())
        gameThread.join();

    if (gameState == ENDED)
        console_log("Successfully ended gameLoop");
    else
        throw std::runtime_error("Failed to end game Loop");
}

void Game::loop()
{
    gameState = RUNNNING;

    spawnTetromino();

    while (gameState != ENDING)
    {
        processIncommingInputs();

        if (levelData.hasGravityIntervalElapsed())
            updateGame(TetrisAction::GRAVITY);

        broadcastBoardIfChanges();
        // board.printStatus();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    gameState = ENDED;
    broadcastEndGameStatus();
}

int Game::calculatePoints(int nLines, int level)
{
    int x = std::min(nLines, 4);
    int P_x = ((280 * (x * x * x) - 1470 * (x * x) + 2630 * (x)-1320) / 3);

    return (nLines > 0) ? P_x * (level + 1) : 0;
}

void Game::enqueueInput(Player *player, TetrisAction action)
{
    player->enqueueAction(action);
}

void Game::processIncommingInputs()
{
    TetrisAction action;
    for (Player *pl : players)
        while (pl->popAction(action))
            updateGame(action);
}

void Game::updateGame(TetrisAction lastAction)
{
    if (board.reachedTop())
    {
        gameState = ENDING;
        std::cout << "(GAME OVER) !!!" << std::endl;
        std::cout << "(Final score): " << levelData.getScore() << std::endl;
        std::cout << "You played till level: " << levelData.getLevel() << std::endl;
        return;
    }

    // Clear old tetrominos before processing the new action
    board.clearFallingTetrominos();

    // Handle tetromino collision
    if (board.tryMove(*currentTetromino, lastAction))
    {
        // Place the tetromino if it has fallen or the action was DROP_FASTER
        if (lastAction == TetrisAction::DROP_FASTER || lastAction == TetrisAction::GRAVITY)
        {
            lockTetromino();
            tryClearFullLines();

            if (levelData.tryLevelUp(LEVEL_UP_LINES))
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

void Game::tryClearFullLines()
{
    levelData.addClearedLines(board.clearLines());
    levelData.addScore(calculatePoints(levelData.getLinesClearedThisLevel(), levelData.getLevel()));
}

void Game::broadcastBoardIfChanges() const
{
    if (!currentTetromino->shouldBroadcastState())
        return;
    packetSender->sendPacket(Packet(PacketType::GAME_SCREEN, board.constructBoardJsonToBroadcast(), nullptr));
}

void Game::broadcastEndGameStatus() const
{
    if (gameState != GameState::ENDED)
        throw std::logic_error("Game is still running, why would you broadvst this status?");

    EndGameData endGame;
    endGame.linesRemoved = levelData.getTotalLinesCleared();
    endGame.totalPoints = levelData.getScore();
    endGame.finalLevel = levelData.getLevel();
    endGame.gameTime = -1;

    for (Player *pl : players)
        endGame.players.emplace_back(pl->getData());

    packetSender->sendPacket((Packet(PacketType::ENG_GAME_SCREEN, endGame.serialize(), nullptr)));
}