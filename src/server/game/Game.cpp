#include "Game.hpp"
#include <tetrominoFactory.hpp>
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
    lastGravityTick = std::chrono::steady_clock::now();

    while (gameState != GameState::ENDING)
    {
        TetrisAction action;

        for (Player *pl : players)
            while (pl->actions.pop(action))
                updateGame(action);

        updateGame(TetrisAction::EMPTY);
        sendBoardUpdates();
        // board.printStatus();

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    gameState = GameState::ENDED;
}

void Game::updateGame(TetrisAction lastAction)
{
    if (board.reachedTop())
    {
        std::cout << "(GAME OVER) !!!" << std::endl;
        std::cout << "(Final score) : " << score << " !!!" << std::endl;
        std::cout << "You played till level: " << level << std::endl;

        board.clear();

        gameState = GameState::ENDING;
        return;
    }

    // Clear old tetrominos
    board.clearFallingTetrominos();

    // Check if it's time to apply gravity
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastGravityTick).count() > minTimeMs)
    {
        currentTetromino->dropGravity();
        lastGravityTick = now;
    }

    // Check collision after gravity or moves
    if (board.checkCollision(*currentTetromino, lastAction))
    {
        if (lastAction == TetrisAction::DROP_FASTER || currentTetromino->getGravity())
        {
            // Place and lock tetromino
            // std::cout << "Last move received = " << TetrisActionToString(lastAction) << " e gravidade = " << currentTetromino->getGravity() << std::endl;
            board.placeTetromino(*currentTetromino, true);

            // GRAVIDADE TEM Q SAIR DO TETROMINO, é uyma propriedade GLOBAL agr
            currentTetromino->gravity = false;

            // std::cout << "Encaixei" << std::endl;
            currentTetromino.reset();

            nLinesClearedThisLevel += board.clearLines();
            score += calculateLinesToPoints(nLinesClearedThisLevel, level);

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

Game::Game(PacketSender *sender) : Debuggable("Game"), packetSender(sender), this_instance(instanceCount++)
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
    if (!board.anyChanges())
        return;

    nlohmann::json boardJson = board.constructBoardJsonToBroadcast();
    sendBoardState(boardJson);
}

void Game::sendBoardState(const nlohmann::json &boardJson) const
{
    packetSender->sendPacket(Packet(PacketType::GAME_SCREEN, boardJson, nullptr));
}