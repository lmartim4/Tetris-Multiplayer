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
#include "MiniBoardRenderer.hpp"
#include "BoardRenderer.hpp"
#include "GameStatusRenderer.hpp"

class BoardScreen : public Screen
{
private:
    std::shared_ptr<ClientManager> clientManager;

    std::shared_ptr<TetrisBoard> board;
    
    std::shared_ptr<MiniBoardRenderer> miniBoard;
    std::shared_ptr<BoardRenderer> mainBoard;
    std::shared_ptr<GameStatusRenderer> statusDisplay;

    std::mutex renderMutex;

public:
    BoardScreen(sf::RenderWindow &window, std::shared_ptr<ClientManager> clientManager);

    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void render(sf::RenderWindow &window) override;
    void update(float deltaTime) override;
    void handleKeyPress(sf::Event event);
};
