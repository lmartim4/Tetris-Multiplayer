#include "BoardScreen.hpp"

void BoardScreen::setupRenderers()
{
    renderGrid.clear();

    for (int x = 0; x < board.getHeight(); ++x)
    {
        std::vector<std::shared_ptr<CellRenderer>> row;
        for (int y = 0; y < board.getWidth(); ++y)
        {
            auto cell = std::make_shared<CellRenderer>(
                sf::Vector2f(CELL_SIZE, CELL_SIZE),
                sf::Vector2f(y * CELL_SIZE, x * CELL_SIZE),
                sf::Color::Red, board.getGrid()[x][y]);

            row.push_back(cell);
        }
        renderGrid.push_back(row);
    }
}

BoardScreen::BoardScreen(ClientManager &clientManager) : clientManager(clientManager), board(3, 4)
{
    setupRenderers();
}

void BoardScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    handleKeyPress(event);

    for (auto &row : renderGrid)
        for (auto &cell : row)
            cell->handleEvent(event);
}

void BoardScreen::render(sf::RenderWindow &window)
{
    for (auto &row : renderGrid)
        for (auto &renderCell : row)
        {
            renderCell->updateData();
            window.draw(*renderCell);
        }
}

void BoardScreen::updateBoardFromJson(const nlohmann::json &boardData)
{
    board.deserialize(boardData);

    // board.printDebug();

    if (board.getHeight() != renderGrid.size() || (board.getWidth() != renderGrid[0].size()))
        setupRenderers();
}

void BoardScreen::update(float deltaTime)
{
    nlohmann::json lastBoard;
    if (clientManager.hasBoard(lastBoard))
        updateBoardFromJson(lastBoard);
}

void BoardScreen::handleKeyPress(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
        clientManager.onPressKey(event.key);
}