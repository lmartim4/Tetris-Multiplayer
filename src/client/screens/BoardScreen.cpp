#include "BoardScreen.hpp"

BoardScreen::BoardScreen(sf::RenderWindow &window, std::shared_ptr<ClientManager> clientManager)
    : Screen(window), clientManager(clientManager)
{
    miniBoard = std::make_shared<MiniBoardRenderer>(sf::Vector2f(100.f, 100.f), 50.f);
    statusDisplay = std::make_shared<GameStatusRenderer>(defaultFont);
    board = std::make_shared<TetrisBoard>(2, 2);
    mainBoard = std::make_shared<BoardRenderer>(clientManager, board);
    
    miniBoard->refreshPosition(window);
    statusDisplay->updateTextPositions(window);
}

void BoardScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    std::lock_guard<std::mutex> lock(renderMutex);

    if (event.type == sf::Event::Resized)
    {
        mainBoard->onWindowResize(window);
        statusDisplay->updateTextPositions(window);
    }

    handleKeyPress(event);
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

    statusDisplay->updateGameStatus(clientManager->getGameData());

    std::optional<Tetromino> t = clientManager->getNextTetromino();

    if (t.has_value())
        miniBoard->setTetromino(t.value(), CellRenderMode::CentralGradient);
}

void BoardScreen::handleKeyPress(sf::Event event)
{
    if (event.type == sf::Event::Resized)
        miniBoard->refreshPosition(window);

    if (event.type == sf::Event::KeyPressed)
        clientManager->onPressKey(event.key);
}

void BoardScreen::renderMiniBoard(sf::RenderTarget &target)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    window.draw(*miniBoard);
}

void BoardScreen::renderMainBoard(sf::RenderTarget &target)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    window.draw(*mainBoard);
}

void BoardScreen::renderGameStatus(sf::RenderTarget &target)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    target.draw(*statusDisplay);
}
