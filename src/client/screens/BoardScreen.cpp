#include "BoardScreen.hpp"

BoardScreen::BoardScreen(sf::RenderWindow &window, std::shared_ptr<ClientManager> clientManager)
    : Screen(window), clientManager(clientManager), nextTetrominoBoard(sf::Vector2f(100.f, 100.f), 50.f),
      score(defaultFont, "Score: 0", sf::Color::White, sf::Vector2f(400, 400), 24),
      lines(defaultFont, "Lines: 0", sf::Color::White, sf::Vector2f(400, 350), 24),
      level(defaultFont, "Level: 0", sf::Color::White, sf::Vector2f(400, 250), 24)
{
    board = std::make_shared<TetrisBoard>(2, 2);
    mainBoard = std::make_shared<BoardRenderer>(clientManager, board);

    updateTextPositions();
    nextTetrominoBoard.refreshPosition(window);
}

void BoardScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    
    if (event.type == sf::Event::Resized)
    {
        mainBoard->onWindowResize(window);
        updateTextPositions();
    }

        handleKeyPress(event);

    for (auto &row : renderGrid)
        for (auto &cell : row)
            cell->handleEvent(event, window);
}

void BoardScreen::render(sf::RenderWindow &window)
{
    sf::View defaultView(sf::FloatRect(
        0.f, 0.f,
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)));

    window.setView(defaultView);

    renderGameStatus(window);
    renderMainBoard(window);
    renderMiniBoard(window);
}

void BoardScreen::update(float deltaTime)
{
    nlohmann::json lastBoard;

    if (clientManager->hasBoard(lastBoard))
        mainBoard->updateBoardFromJson(lastBoard, window);

    std::optional<Tetromino> t = clientManager->getNextTetromino();

    if (t.has_value())
        nextTetrominoBoard.setTetromino(t.value(), CellRenderMode::CentralGradient);
}

void BoardScreen::handleKeyPress(sf::Event event)
{
    if (event.type == sf::Event::Resized)
    {
        nextTetrominoBoard.refreshPosition(window);
    }

    if (event.type == sf::Event::KeyPressed)
        clientManager->onPressKey(event.key);
}

void BoardScreen::updateTextPositions()
{
    // Window dimensions (in pixels, since we set the defaultView).
    float w = static_cast<float>(window.getSize().x);
    float h = static_cast<float>(window.getSize().y);

    // Margins
    float marginLeft = 20.f;  // distance from the left of the right region
    float marginRight = 20.f; // distance from the right edge of the window

    // The "right side" region is from 80% of the width up to the window’s right edge
    float regionLeft = w * 0.8f + marginLeft;
    float regionRight = w - marginRight;

    // Bottom margin start (for stacking the texts vertically upward)
    float marginBottom = h - 150.f;

    // Example: place them in increments of 50px
    lines.setPosition(regionLeft, marginBottom);
    level.setPosition(regionLeft, marginBottom + 50.f);
    score.setPosition(regionLeft, marginBottom + 100.f);
}

void BoardScreen::renderMiniBoard(sf::RenderWindow &window)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    window.draw(nextTetrominoBoard);
}

void BoardScreen::renderMainBoard(sf::RenderWindow &window)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    window.draw(*mainBoard);
}

void BoardScreen::renderGameStatus(sf::RenderWindow &window)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    window.draw(lines);
    window.draw(level);
    window.draw(score);
}
