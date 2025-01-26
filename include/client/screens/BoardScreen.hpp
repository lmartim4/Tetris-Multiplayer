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
#include "BoardRenderer.hpp"

class BoardScreen : public Screen
{
private:
    std::shared_ptr<TetrisBoard> board;
    MiniBoard nextTetrominoBoard;
    std::shared_ptr<BoardRenderer> mainBoard;

    std::shared_ptr<ClientManager> clientManager;
    std::vector<std::vector<std::shared_ptr<CellRenderer>>> renderGrid;

    Button score, lines, level;

    std::mutex renderMutex;

    void updateTextPositions();

    void renderMiniBoard(sf::RenderWindow &window);
    void renderMainBoard(sf::RenderWindow &window);
    void renderGameStatus(sf::RenderWindow &window);

public:
    BoardScreen(sf::RenderWindow &window, std::shared_ptr<ClientManager> clientManager);

    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void render(sf::RenderWindow &window) override;
    void update(float deltaTime) override;
    void handleKeyPress(sf::Event event);
};
