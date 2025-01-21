#include "Game.hpp"
#include "TetrominoFactory.hpp"
#include "ServerManager.hpp"
#include "Clock.hpp"

#define LEVEL_UP_LINES 10

int Game::instanceCount = 0;

Game::Game(ServerManager &server) : server(server), board(16, 10), this_instance(instanceCount++)
{
    gameState = WAITING_PLAYERS;
    boardController = new TetrisBoardController(board);
    logger = new Debuggable("Game");

    logger->console_log("Initializing Game (" + std::to_string(this_instance) + ")...");
}

Game::~Game()
{
    endGameLoop();
    logger->console_log("Destroying Game (" + std::to_string(this_instance) + ")...");
}

void Game::addPlayer(Player *player)
{
    if (gameState != WAITING_PLAYERS)
        throw std::logic_error("Cannot add player (not waiting for players)");
    players.emplace_back(player);
}

void Game::spawnNextTetromino(Player *player)
{
    logger->console_log("spawnNextTetromino");
    // Must have a previus next tetromino
    if (nextTetromino.find(player) == nextTetromino.end())
        nextTetromino.emplace(player, TetrominoFactory::createTetromino());

    currentTetromino.erase(player);
    currentTetromino.emplace(player, nextTetromino.find(player)->second);

    nextTetromino.erase(player);
    nextTetromino.emplace(player, TetrominoFactory::createTetromino());
}

void Game::startGame()
{
    static int instanceCount = 0;

    if (gameState != WAITING_PLAYERS)
        throw std::logic_error("Cannot start a game that is not currently waiting for players");

    gameState = STARTING;
    logger->console_log("Starting Game (" + std::to_string(instanceCount++) + ")...");

    gameThread = std::thread(&Game::loop, this);
}

void Game::endGameLoop()
{
    if (gameThread.joinable())
        gameThread.join();

    if (gameState == ENDED)
        logger->console_log("Successfully ended gameLoop");
    else
        throw std::runtime_error("Failed to end game Loop");
}

void Game::loop()
{
    gameState = RUNNNING;

    for (Player *pl : players)
        spawnNextTetromino(pl);

    while (gameState != ENDING)
    {
        processIncommingInputs();

        if (gravity.hasGravityIntervalElapsed())
            for (const auto &pair : currentTetromino)
                updateGame(pair.second, TetrisAction::GRAVITY);

        broadcastBoardIfChanges();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    gameState = ENDED;
    broadcastEndGameStatus();
}

void Game::updateGame(std::shared_ptr<Tetromino> tetromino, TetrisAction action)
{
    if (!board.reachedTop())
    {
        boardController->clearFallingTetromino(*tetromino);

        if (action == TetrisAction::HARD_DROP)
        {
            while (!(boardController->checkCollision(*tetromino, TetrisAction::GRAVITY)))
            {
            }

            tetrominoHasFallen(tetromino);
        }
        else if (boardController->checkCollision(*tetromino, action))
        {
            if (action == TetrisAction::GRAVITY || action == TetrisAction::HARD_DROP)
            {
                // Place the tetromino was it finally dropped and locked
                tetrominoHasFallen(tetromino);
            }
            else
            {
                // Invalid move, revert the tetromino state
                boardController->placeTetromino(*tetromino, false);
                server.broadcastSound(SoundType::DenyErrorSound);
            }
        }
        else
        {
            // Valid Move
            boardController->placeTetromino(*tetromino, false);
            server.broadcastSound(SoundType::FabricImpactSound);
        }
    }
    else
    {
        gameState = ENDING;
        logger->console_log("Ending Game");
        server.broadcastSound(SoundType::DeathSound);
    }
}

void Game::lockTetromino(std::shared_ptr<Tetromino> tetromino)
{
    server.broadcastSound(SoundType::DjembeSlap);
    boardController->placeTetromino(*tetromino, true);
    tetromino.reset();
}

int Game::clearFullLines()
{
    int clearedLines = boardController->clearFullLines();

    if (clearedLines > 0)
    {
        gameData.addLinesClearedOnThisLevel(clearedLines);
        gameData.addTotalLinesCleared(clearedLines);
        server.broadcastSound(SoundType::BreakLine);
    }
    return clearedLines;
}

void Game::broadcastBoardIfChanges() const
{
    bool send = false;

    for (const auto &pair : currentTetromino)
    {
        if (pair.second->shouldBroadcastState())
        {
            send = true;
            break;
        }
    }

    if (!send)
        return;

    board.printDebug();
    // static int boardsSent = 0;
    // logger->console_log("Sent " + std::to_string(boardsSent++) + "\n");
    server.sendPacket(Packet(PacketType::GAME_SCREEN, board, nullptr));
}

void Game::broadcastEndGameStatus() const
{
    if (gameState != GameState::ENDED)
        throw std::logic_error("Game is still running, why would you broadvst this status?");

    EndGameData endGame;
    endGame.linesRemoved = gameData.getTotalLinesCleared();
    endGame.totalPoints = gameData.getScore();
    endGame.finalLevel = gameData.getLevel();
    endGame.gameTime = -1;

    for (Player *pl : players)
        endGame.players.emplace_back(pl->getData());

    server.sendPacket((Packet(PacketType::ENG_GAME_SCREEN, endGame, nullptr)));
}

void Game::processIncommingInputs()
{
    TetrisAction action;
    for (Player *pl : players)
        while (pl->popAction(action))
            updateGame(currentTetromino.at(pl), action);
}

int Game::calculatePoints(int nLines, int level)
{
    int x = std::min(nLines, 4);
    int P_x = ((280 * (x * x * x) - 1470 * (x * x) + 2630 * (x)-1320) / 3);

    return (nLines > 0) ? P_x * (level + 1) : 0;
}

void Game::tetrominoHasFallen(std::shared_ptr<Tetromino> tetromino)
{
    lockTetromino(tetromino);
    int justClearedLines = clearFullLines();

    gameData.addScore(calculatePoints(justClearedLines, gameData.getLevel()));

    if (gameData.tryLevelUp(LEVEL_UP_LINES))
    {
        boardController->clearFallenTetrominos();
        logger->console_log("Level Up: (" + std::to_string(gameData.getLevel()) + ")");
        server.broadcastSound(SoundType::LevelUp);
    }

    server.sendPacket(Packet(PacketType::GAME_SCORE, gameData, nullptr));

    // TODO: Better organize the logic here

    Player *foundKey = nullptr;
    for (const auto &pair : currentTetromino)
    {
        if (pair.second == tetromino)
        {
            foundKey = pair.first;
            break;
        }
    }

    spawnNextTetromino(foundKey);
}