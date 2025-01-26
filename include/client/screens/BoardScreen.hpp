#pragma once

#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include "Screen.hpp"
#include "Button.hpp"
#include "CellRenderer.hpp"
#include "TetrisBoard.hpp"
#include "ClientManager.hpp"
#include "MiniBoard.hpp"

class BoardScreen : public Screen
{
private:
    TetrisBoard board;
    MiniBoard nextTetrominoBoard;

    ClientManager &clientManager;
    std::vector<std::vector<std::shared_ptr<CellRenderer>>> renderGrid;

    Button score, lines, level;

    std::mutex renderMutex;

    float computeCellSize() const;

    void createRenders();
    void refreshAllCellRenders();
    void updateTextPositions();

    void renderMiniBoard(sf::RenderWindow &window);
    void renderMainBoard(sf::RenderWindow &window);
    void renderGameStatus(sf::RenderWindow &window);

public:
    BoardScreen(sf::RenderWindow &window, ClientManager &clientManager);

    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void render(sf::RenderWindow &window) override;
    void updateBoardFromJson(const nlohmann::json &boardData);
    void update(float deltaTime) override;
    void handleKeyPress(sf::Event event);
};
