#include "Game.hpp"
#include "TetrominoFactory.hpp"
#include "ServerManager.hpp"
#include "Clock.hpp"

#define LEVEL_UP_LINES 10

int Game::instanceCount = 0;

Game::Game(ServerManager &server) : server(server), board(16, 10), this_instance(instanceCount++)
{
    logger = new Debuggable("Game");
    logger->console_log("Initializing Game (" + std::to_string(this_instance) + ")...");
    gameState = WAITING_PLAYERS;
    boardController = new TetrisBoardController(board);
}

Game::~Game()
{
    endGameLoop();
    logger->console_log("Destroying Game (" + std::to_string(this_instance) + ")...");
}

void Game::addPlayer(Player *player)
{
    if (gameState != WAITING_PLAYERS)
    {
        logger->console_log("Cannot add player (not waiting for players)");
        return;
    }

    players.emplace_back(player);
}

void Game::spawnTetromino()
{
    currentTetromino = TetrominoFactory::createTetromino();
}

void Game::startGame()
{
    static int instanceCount = 0;

    if (gameState != WAITING_PLAYERS)
    {
        std::cerr << "Cannot start a game that is not currently waiting for players" << std::endl;
        return;
    }

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
    spawnTetromino();
    while (gameState != ENDING)
    {
        processIncommingInputs();
        if (gravity.hasGravityIntervalElapsed())
            updateGame(TetrisAction::GRAVITY);

        broadcastBoardIfChanges();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    gameState = ENDED;
    broadcastEndGameStatus();
}

void Game::updateGame(TetrisAction action)
{
    if (!board.reachedTop())
    {
        boardController->clearFallingTetrominos();
        if (action == TetrisAction::HARD_DROP)
        {
            while (!(boardController->checkCollision(*currentTetromino, TetrisAction::GRAVITY)))
            {
            }
            tetrominoHasFallen();
        }
        else if (boardController->checkCollision(*currentTetromino, action))
        {
            if (action == TetrisAction::GRAVITY || action == TetrisAction::HARD_DROP)
            {
                // Place the tetromino was it finally dropped and locked
                tetrominoHasFallen();
            }
            else
            {
                // Invalid move, revert the tetromino state
                boardController->placeTetromino(*currentTetromino, false);
                server.broadcastSound(SoundType::DenyErrorSound);
            }
        }
        else
        {
            // Valid Move
            boardController->placeTetromino(*currentTetromino, false);
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

void Game::lockTetromino()
{
    server.broadcastSound(SoundType::DjembeSlap);
    boardController->placeTetromino(*currentTetromino, true);
    currentTetromino.reset();
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
    if (!currentTetromino->shouldBroadcastState())
        return;

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
            updateGame(action);
}

int Game::calculatePoints(int nLines, int level)
{
    int x = std::min(nLines, 4);
    int P_x = ((280 * (x * x * x) - 1470 * (x * x) + 2630 * (x)-1320) / 3);

    return (nLines > 0) ? P_x * (level + 1) : 0;
}

void Game::tetrominoHasFallen()
{
    lockTetromino();
    int justClearedLines = clearFullLines();

    gameData.addScore(calculatePoints(justClearedLines, gameData.getLevel()));

    if (gameData.tryLevelUp(LEVEL_UP_LINES))
    {
        boardController->clearFallenTetrominos();
        logger->console_log("Level Up: (" + std::to_string(gameData.getLevel()) + ")");
        server.broadcastSound(SoundType::LevelUp);
    }

    server.sendPacket(Packet(PacketType::GAME_SCORE, gameData, nullptr));

    spawnTetromino();
}