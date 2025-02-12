#pragma once

#include "BoardController.hpp"
#include "EndGameData.hpp"
#include "Player.hpp"
#include "GameStatus.hpp"
#include "GamePhysics.hpp"
#include "TetrominoController.hpp"
#include "TetrominoManager.hpp"
#include "Logger.hpp"
#include "ServerManager.hpp"
#include "Clock.hpp"

#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <map>

// #define GAME_DEBUG

enum GameState
{
    INITIALIZING,    // WAITING CLASS CREATION
    WAITING_PLAYERS, // SETTING UP PLAYER LIST
    STARTING,        // STARTING THREADS (not running yet)
    RUNNNING,        // THREADS ARE RUNNING
    ENDING,          // TELL THREADS TO STOP
    ENDED            // EVERYTHING IS ENDED
};

class Game
{
private:
    static int instanceCount;
    const int this_instance;

    std::atomic<bool> stopLoop{false};

    Clock gameTime;

    Logger *logger;

    ServerManager &server;

    std::atomic<GameState> gameState = INITIALIZING;

    GamePhysics physics;
    GameStatus gameData;

    std::shared_ptr<TetrominoManager> tetrominoManager;
    std::shared_ptr<TetrominoController> tetrominoController;
    std::shared_ptr<BoardController> boardController;
    std::shared_ptr<TetrisBoard> board;

    std::vector<Player *> players;

    void processPlayersActions();

    void loop();

    void processAction(Player *player, std::shared_ptr<Tetromino> tetromino, TetrisAction action);

    void trySpawnTetromino(Player *player);

    void checkMusicLimits();

    void broadcastBoardIfChanges() const;
    void broadcastEndGameStatus() const;

    int calculatePoints(int nLines, int level);
    void checkForPlacedTetrominos();

    std::thread gameThread;

public:
    Game(ServerManager &sender);
    ~Game();

    GameState getState() { return gameState; }

    void addPlayer(Player *player);
    void startGame();
    void endGameLoop(bool force);
};