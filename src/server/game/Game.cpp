#include "Game.hpp"
#include "TetrominoFactory.hpp"
#include "ServerManager.hpp"
#include "Clock.hpp"

#define LEVEL_UP_LINES 10

int Game::instanceCount = 0;

#pragma region startup_logic

Game::Game(ServerManager &server) : server(server), this_instance(instanceCount++), physics(server)
{
    gameState = WAITING_PLAYERS;

    logger = new Logger("Game");

    board = std::make_shared<TetrisBoard>(1, 1);
    tetrominoController = std::make_shared<TetrominoController>();
    tetrominoManager = std::make_shared<TetrominoManager>();
    boardController = std::make_shared<BoardController>(board);

    board->clearBoard();
    logger->console_log("Initializing Game (" + std::to_string(this_instance) + ")...");
}

Game::~Game()
{
    endGameLoop(false);
    logger->console_log("Destroying Game (" + std::to_string(this_instance) + ")...");

    boardController.reset();
    tetrominoManager.reset();
    tetrominoController.reset();
    board.reset();

    delete logger;
}

void Game::addPlayer(Player *player)
{
    if (gameState != WAITING_PLAYERS)
        throw std::logic_error("Cannot add player (not waiting for players)");

    players.emplace_back(player);

    // board size definition
    board->setSize(16 + 2 * (players.size() - 1), 10 + 4 * (players.size() - 1));
}

void Game::startGame()
{
    gameTime.start();
    static int instanceCount = 0;

    if (gameState != WAITING_PLAYERS)
        throw std::logic_error("Cannot start a game that is not currently waiting for players");

    gameState = STARTING;
    logger->console_log("Starting Game (" + std::to_string(instanceCount++) + ")...");

    gameThread = std::thread(&Game::loop, this);
}

void Game::endGameLoop(bool force)
{
    if (force)
        stopLoop = true;

    while (gameState != ENDED)
    {
    }

    if (gameThread.joinable())
        gameThread.join();

    if (gameState == ENDED)
        logger->console_log("Successfully ended game Loop");
    else
        throw std::runtime_error("Failed to end game Loop");
}

#pragma endregion

#pragma region util_logic

int Game::calculatePoints(int nLines, int level)
{
    int x = std::min(nLines, 4);
    int P_x = ((280 * (x * x * x) - 1470 * (x * x) + 2630 * (x)-1320) / 3);
    return (nLines > 0) ? P_x * (level + 1) : 0;
}

void Game::trySpawnTetromino(Player *player)
{
    if (tetrominoManager->spawnNextTetromino(player, boardController, tetrominoController, board->getWidth()))
    {
        server.sendPacket(Packet(PacketType::NEXT_TETROMINO_DATA, *tetrominoManager->getNextTetromino(player), server.getEnetPeerByPlayer(player)));
        return;
    }

    gameState = GameState::ENDING;
    logger->console_log("Ending Game");
    server.broadcastSound(SoundType::DeathSound);
}

void Game::processPlayersActions()
{
    TetrisAction action;
    for (Player *pl : players)
        while (pl->popAction(action))
            processAction(pl, tetrominoManager->getCurrentTetromino(pl), action);
}

void Game::broadcastBoardIfChanges() const
{
    bool send = false;

    for (const auto tetromino : tetrominoManager->getCurrentTetrominos())
    {
        if (tetromino->shouldBroadcastState())
        {
            send = true;
            break;
        }
    }

    if (!send)
        return;

#ifdef GAME_DEBUG
    board->printDebug();
#endif

    server.sendPacket(Packet(PacketType::GAME_SCREEN, *board, nullptr));
}

void Game::broadcastEndGameStatus() const
{
    if (gameState != GameState::ENDED)
        throw std::logic_error("Game is still running, why would you broadvst this status?");

    EndGameData endGame;

    endGame.linesRemoved = gameData.getTotalLinesCleared();
    endGame.totalPoints = gameData.getScore();
    endGame.finalLevel = gameData.getLevel();
    endGame.gameTime = gameTime.elapsedMicroseconds() / (1000 * 1000);

    for (Player *pl : players)
        endGame.players.emplace_back(pl->getData());

    server.sendPacket((Packet(PacketType::ENG_GAME_SCREEN, endGame, nullptr)));
    server.broadcastBackgroundSound(SoundType::STOP); // Looping
}

#pragma endregion

void Game::processAction(Player *player, std::shared_ptr<Tetromino> tetromino, TetrisAction action)
{
    if (tetromino == nullptr)
        throw std::runtime_error("Tetromino must not be null");

    CollisionType col;

    if (action == TetrisAction::HARD_DROP)
        col = physics.applyHardDrop(tetromino, boardController, tetrominoController, tetrominoManager);
    else
        col = physics.applyAction(tetromino, action, boardController, tetrominoController, tetrominoManager);

    if (col == CollisionType::FALLING_OTHER || col == CollisionType::NONE)
        return;
}

void Game::loop()
{
    gameState = RUNNNING;

    for (Player *pl : players)
        trySpawnTetromino(pl);

    while (gameState != ENDING && !stopLoop)
    {
        processPlayersActions();
        physics.applyGravity(boardController, tetrominoController, tetrominoManager);

        checkForPlacedTetrominos();
        broadcastBoardIfChanges();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    gameState = ENDED;
    broadcastEndGameStatus();
}

void Game::checkForPlacedTetrominos()
{
    int clearedLines = boardController->findAndClearFullLines();

    for (auto t : tetrominoManager->getCurrentTetrominos())
        if (tetrominoController->isLockedInPlace(t))
            trySpawnTetromino(tetrominoManager->getPlayerByTetromino(t));

    checkMusicLimits();

    if (clearedLines > 0)
    {
        gameData.addLinesClearedOnThisLevel(clearedLines);
        gameData.addTotalLinesCleared(clearedLines);
        server.broadcastSound(SoundType::BreakLine);
    }

    gameData.addScore(calculatePoints(clearedLines, gameData.getLevel()));
    server.sendPacket(Packet(PacketType::GAME_SCORE, gameData, nullptr));

    if (gameData.shouldLevelUp())
    {
        physics.increaseGravityDifficulty();
        boardController->clearFallenTetrominos();
        server.broadcastSound(SoundType::LevelUp);
    }
}

void Game::checkMusicLimits()
{
    double completed = (board->getHighestCell() * 100.0f) / (board->getHeight() * 1.0f);

    const double thresholds[] = {25.f, 60.f, 70.f, 90.f};
    static int lastThresholdIndex = -1;

    int currentThresholdIndex = -1;
    for (int i = 0; i < 4; ++i)
    {
        if (completed < thresholds[i])
        {
            currentThresholdIndex = i;
            break;
        }
    }

    if (currentThresholdIndex != lastThresholdIndex)
    {
        lastThresholdIndex = currentThresholdIndex;

        switch (currentThresholdIndex)
        {
        case 0:
            server.broadcastBackgroundSound(SoundType::Lv2);
            break;
        case 1:
            server.broadcastBackgroundSound(SoundType::Lv3);
            break;
        case 2:
            server.broadcastBackgroundSound(SoundType::Lv4);
            break;
        case 3:
            server.broadcastBackgroundSound(SoundType::Lv256);
            break;
        default:
            break;
        }
    }
}
