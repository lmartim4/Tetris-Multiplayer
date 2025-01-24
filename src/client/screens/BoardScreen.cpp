#include "BoardScreen.hpp"

void BoardScreen::setupRenderers()
{
    std::lock_guard<std::mutex> lock(renderMutex);

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
    // 2. Configure our score text
    score.setFont(defaultFont);
    score.setString("Score: 0"); // Mocked-up text
    score.setCharacterSize(24);  // Pixel size
    score.setFillColor(sf::Color::White);
    score.setPosition(400, 400); // Position on the screen

    // 3. Configure our lines text
    lines.setFont(defaultFont);
    lines.setString("Lines: 0"); // Mocked-up text
    lines.setCharacterSize(24);
    lines.setFillColor(sf::Color::White);
    lines.setPosition(400, 350);

    // 4. Configure our level text
    level.setFont(defaultFont);
    level.setString("Level: 1"); // Mocked-up text
    level.setCharacterSize(24);
    level.setFillColor(sf::Color::White);
    level.setPosition(400, 250);

    setupRenderers();
}

void BoardScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    std::lock_guard<std::mutex> lock(renderMutex);

    handleKeyPress(event);

    for (auto &row : renderGrid)
        for (auto &cell : row)
            cell->handleEvent(event);
}

void BoardScreen::render(sf::RenderWindow &window)
{
    std::lock_guard<std::mutex> lock(renderMutex);

    for (std::vector<std::shared_ptr<CellRenderer>> row : renderGrid)
    {
        for (std::shared_ptr<CellRenderer> renderCell : row)
        {
            renderCell->updateData();
            window.draw(*renderCell);
        }
    }

    window.draw(lines);
    window.draw(level);
    window.draw(score);
}

void BoardScreen::updateBoardFromJson(const nlohmann::json &boardData)
{
    board.deserialize(boardData);
    GameData data = clientManager.getGameData();
    level.setString("Level: " + std::to_string(data.getLevel()));
    lines.setString("Lines: " + std::to_string(data.getTotalLinesCleared()));
    level.setString("Score: " + std::to_string(data.getScore()));

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