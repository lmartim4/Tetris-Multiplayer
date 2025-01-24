#include "Game.hpp"
#include "TetrominoFactory.hpp"
#include "ServerManager.hpp"
#include "Clock.hpp"

#define LEVEL_UP_LINES 10

int Game::instanceCount = 0;

Game::Game(ServerManager &server) : server(server), this_instance(instanceCount++)
{
    board = std::make_shared<TetrisBoard>(16, 10);

    gameState = WAITING_PLAYERS;
    boardController = new TetrisBoardController(board);
    logger = new Logger("Game");

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
    board->setSize(16, 10 + 3 * (players.size() - 1));
}

void Game::spawnNextTetromino(Player *player)
{
    if (nextTetromino.find(player) == nextTetromino.end())
        nextTetromino.emplace(player, TetrominoFactory::createTetromino());

    std::shared_ptr<Tetromino> spawningTetromino = nextTetromino.find(player)->second;

    currentTetromino.erase(player);
    currentTetromino.emplace(player, spawningTetromino);

    int maxTries = board->getWidth();
    CollisionType col = boardController->checkCollision(spawningTetromino, TetrisAction::IDLE);

    while (col != CollisionType::NONE && maxTries-- > 0)
    {
        spawningTetromino->evolveStates(true, TetrisAction::RIGHT);
        col = boardController->checkCollision(spawningTetromino, TetrisAction::IDLE);

        if (maxTries == 0)
        {
            gameState = GameState::ENDING;
            logger->console_log("Ending Game");
            server.broadcastSound(SoundType::DeathSound);
        }
    }

    boardController->setCellState(spawningTetromino, CellState::FALLING);

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
            processGravity();

        broadcastBoardIfChanges();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    gameState = ENDED;
    broadcastEndGameStatus();
}

void Game::updateGame(std::shared_ptr<Tetromino> tetromino, TetrisAction action)
{

    boardController->clearFallingTetromino(tetromino);

    CollisionType colision;

    if (action == TetrisAction::HARD_DROP)
    {
        do
        {

            colision = boardController->checkCollision(tetromino, TetrisAction::GRAVITY);
            if (colision == CollisionType::NONE)
                tetromino->evolveStates(true, TetrisAction::GRAVITY);

        } while (colision == CollisionType::NONE);

        boardController->setCellState(tetromino, CellState::FALLING);
        onTetrominoColide(tetromino, colision);
        return;
    }

    colision = boardController->checkCollision(tetromino, action);

    if (colision != CollisionType::NONE)
    {
        // Invalid move, revert the tetromino state
        boardController->setCellState(tetromino, CellState::FALLING);
        server.broadcastSound(SoundType::DenyErrorSound);
    }
    else
    {
        // Valid Move
        tetromino->evolveStates(true, action);
        boardController->setCellState(tetromino, CellState::FALLING);
        server.broadcastSound(SoundType::FabricImpactSound);
    }
}

int Game::tryClearFullLines()
{
    int clearedLines = boardController->findAndClearFullLines();

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
    endGame.gameTime = -1;

    for (Player *pl : players)
        endGame.players.emplace_back(pl->getData());

    server.sendPacket((Packet(PacketType::ENG_GAME_SCREEN, endGame, nullptr)));
}

int Game::countNewLockedTetrominos(std::vector<std::shared_ptr<Tetromino>> tetrominos)
{
    int count = 0;

    for (const std::shared_ptr<Tetromino> tetromino : tetrominos)
    {
        CollisionType col = boardController->checkCollision(tetromino, TetrisAction::GRAVITY);

        if (col == CollisionType::FALLEN_FIXED || col == CollisionType::GROUND)
            if (!tetromino->lockedInPlace)
                count++;
    }

    return count;
}

void Game::processGravity()
{
    CollisionType colision;

    std::vector<std::shared_ptr<Tetromino>> tetrominosToApplyGravity;

    for (const auto &pair : currentTetromino)
    {
        tetrominosToApplyGravity.emplace_back(pair.second);
        pair.second->canMove = false;
        pair.second->lockedInPlace = false;
    }

    while (countNewLockedTetrominos(tetrominosToApplyGravity) > 0)
    {

        for (const std::shared_ptr<Tetromino> tetromino : tetrominosToApplyGravity)
        {
            if (tetromino->lockedInPlace)
                continue;

            colision = boardController->checkCollision(tetromino, TetrisAction::GRAVITY);

            if (colision == CollisionType::GROUND || colision == CollisionType::FALLEN_FIXED)
            {
                tetromino->canMove = false;

                boardController->setCellState(tetromino, CellState::FALLING);
                onTetrominoColide(tetromino, colision);
            }
            else if (colision == CollisionType::FALLING_OTHER || colision == CollisionType::NONE)
            {
                tetromino->canMove = true;
            }
        }
    }

    for (const std::shared_ptr<Tetromino> tetromino : tetrominosToApplyGravity)
    {

        CollisionType col = boardController->checkCollision(tetromino, TetrisAction::GRAVITY);

        if (col == CollisionType::FALLING_OTHER || col == CollisionType::NONE)
        {
            boardController->clearFallingTetromino(tetromino);
            tetromino->evolveStates(true, TetrisAction::GRAVITY);
            boardController->setCellState(tetromino, CellState::FALLING);
        }
    }
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

void Game::onTetrominoColide(std::shared_ptr<Tetromino> tetromino, CollisionType col)
{
    if (col == CollisionType::FALLING_OTHER)
        return;
    
    int justClearedLines = tryClearFullLines();

    tetromino->lockedInPlace = true;
    server.broadcastSound(SoundType::DjembeSlap);
    boardController->setCellState(tetromino, CellState::FALLEN);

    gameData.addScore(calculatePoints(justClearedLines, gameData.getLevel()));

    server.sendPacket(Packet(PacketType::GAME_SCORE, gameData, nullptr));

    if (gameData.shouldLevelUp())
    {
        gravity.increaseGravityDifficulty();
        boardController->clearFallenTetrominos();
        logger->console_log("Level Up: (" + std::to_string(gameData.getLevel()) + ")");
        server.broadcastSound(SoundType::LevelUp);
    }

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