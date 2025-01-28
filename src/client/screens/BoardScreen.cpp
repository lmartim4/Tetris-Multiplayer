#include "BoardScreen.hpp"

BoardScreen::BoardScreen(sf::RenderWindow &window, std::shared_ptr<ClientManager> clientManager)
    : Screen(window), clientManager(clientManager)
{

    setBackground("main_game", "../assets/images/tetris.jpg");
    miniBoard = std::make_shared<MiniBoardRenderer>(sf::Vector2f(100.f, 100.f), 50.f);
    statusDisplay = std::make_shared<GameStatusRenderer>(defaultFont);
    board = std::make_shared<TetrisBoard>(3, 3);
    mainBoard = std::make_shared<BoardRenderer>(clientManager, board, window.getSize());
}

void BoardScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    handleKeyPress(event);
}

void BoardScreen::render(sf::RenderWindow &window)
{
    renderBackground();
    sf::View defaultView(sf::FloatRect(
        0.f, 0.f,
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)));

    window.setView(defaultView);

    std::lock_guard<std::mutex> lock(renderMutex);

    window.draw(*mainBoard);
    window.draw(*miniBoard);
    window.draw(*statusDisplay);
}

void BoardScreen::update(float deltaTime)
{
    std::lock_guard<std::mutex> lock(renderMutex);

    nlohmann::json lastBoard;

    if (clientManager->hasBoard(lastBoard))
        mainBoard->updateBoardFromJson(lastBoard, window);

    statusDisplay->updateGameStatus(clientManager->getGameData());

    std::shared_ptr<Tetromino> t;

    while (clientManager->next_tetrominos.pop(t))
        miniBoard->setTetromino(t, CellRenderMode::VerticalGradient);
}

void BoardScreen::handleKeyPress(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
        clientManager->onPressKey(event.key);
}

void BoardScreen::updateSize(const sf::Vector2u a)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    
    mainBoard->updateSize(a);
    miniBoard->updateSize(a);
    statusDisplay->updateSize(a);
    resizeBackgound(a);
}