#include "BoardScreen.hpp"

void BoardScreen::createRenders()
{
    std::lock_guard<std::mutex> lock(renderMutex);

    renderGrid.clear();

    float CELL_SIZE = computeCellSize();

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

BoardScreen::BoardScreen(sf::RenderWindow &window, ClientManager &clientManager)
    : Screen(window), clientManager(clientManager), board(2, 2),
      score(defaultFont, "Score: 0", sf::Color::White, sf::Vector2f(400, 400), 24),
      lines(defaultFont, "Lines: 0", sf::Color::White, sf::Vector2f(400, 350), 24),
      level(defaultFont, "Level: 1", sf::Color::White, sf::Vector2f(400, 250), 24)
{
    updateTextPositions();
    createRenders();
}

void BoardScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    // Be carefull with mutex inside refreshAllCellRenders()
    if (event.type == sf::Event::Resized)
    {
        refreshAllCellRenders();
        updateTextPositions();
    }

    std::lock_guard<std::mutex> lock(renderMutex);

    handleKeyPress(event);

    for (auto &row : renderGrid)
        for (auto &cell : row)
            cell->handleEvent(event);
}

void BoardScreen::render(sf::RenderWindow &window)
{

    sf::View defaultView(sf::FloatRect(
        0.f, 0.f,
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)));

    window.setView(defaultView);

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
    GameStatus data = clientManager.getGameData();

    level.setString("Level: " + std::to_string(data.getLevel()));
    lines.setString("Lines: " + std::to_string(data.getTotalLinesCleared()));
    score.setString("Score: " + std::to_string(data.getScore()));

    // board.printDebug();

    if (board.getHeight() != renderGrid.size() || (board.getWidth() != renderGrid[0].size()))
        createRenders();
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

float BoardScreen::computeCellSize() const
{
    int boardWidth = board.getWidth();
    int boardHeight = board.getHeight();

    if (boardWidth <= 0 || boardHeight <= 0)
        return 20;

    float maxBoardWidth = window.getSize().x * 0.8;
    float maxBoardHeight = window.getSize().y;

    float cellSizeByWidth = maxBoardWidth / boardWidth;
    float cellSizeByHeight = maxBoardHeight / boardHeight;

    float computedCellSize = std::min(cellSizeByWidth, cellSizeByHeight);

    return computedCellSize;
}

void BoardScreen::refreshAllCellRenders()
{
    std::lock_guard<std::mutex> lock(renderMutex);

    float CELL_SIZE = computeCellSize();

    for (int x = 0; x < board.getHeight(); x++)
        for (int y = 0; y < board.getWidth(); y++)
            renderGrid[x][y]->refreshPosition(sf::Vector2f(CELL_SIZE, CELL_SIZE),
                                              sf::Vector2f(y * CELL_SIZE, x * CELL_SIZE));
}
void BoardScreen::updateTextPositions()
{
    // Window dimensions (in pixels, since we set the defaultView).
    float w = static_cast<float>(window.getSize().x);
    float h = static_cast<float>(window.getSize().y);

    // Margins
    float marginLeft = 20.f;   // distance from the left of the right region
    float marginRight = 20.f;  // distance from the right edge of the window

    // The "right side" region is from 80% of the width up to the window’s right edge
    float regionLeft  = w * 0.8f + marginLeft;
    float regionRight = w - marginRight;

    // Bottom margin start (for stacking the texts vertically upward)
    float marginBottom = h - 150.f;

    // Example: place them in increments of 50px
    lines.setPosition(regionLeft, marginBottom);
    level.setPosition(regionLeft, marginBottom + 50.f);
    score.setPosition(regionLeft, marginBottom + 100.f);

    // This positions them at 'regionLeft' (with 20px margin from 80% point),
    // leaving 20px from the window’s right border.

    // -----------------------------------
    // If instead you wanted them "anchored" on the right edge, 
    // you'd subtract each text's width from 'regionRight'. For example:
    /*
        float textWidth = lines.getLocalBounds().width;
        float textX     = regionRight - textWidth; // anchor on the right
        lines.setPosition(textX, marginBottom);

        // Repeat for level, score, etc.
    */
}
