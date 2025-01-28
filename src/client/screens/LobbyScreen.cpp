#include "LobbyScreen.hpp"

LobbyScreen::LobbyScreen(sf::RenderWindow &window, std::shared_ptr<ClientManager> clientManager)
    : Screen(window),
      clientMan(clientManager)
{
    initializeButtons();

    startGameText->setOnClick([clientManager]()
                              { clientManager->request_game_start(); });
}

void LobbyScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    mainText->handleEvent(event, manager.getWindow());
    startGameText->handleEvent(event, manager.getWindow());

    for (Button ct : clickableTexts)
        ct.handleEvent(event, manager.getWindow());

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        clientMan->disconnect();
}

void LobbyScreen::update(float deltaTime) {}

void LobbyScreen::render(sf::RenderWindow &window)
{
    // Variables for grid layout
    int index = 0;
    int maxColumns = 5; // Max number of circles per row
    int numPlayers = clientMan->getPlayerList().getPlayers().size();

    // Calculate the number of rows needed
    int rows = (numPlayers + maxColumns - 1) / maxColumns;

    // Get the size of the active view (not raw window pixels)
    sf::Vector2f viewSize = window.getView().getSize();

    // Calculate the grid size
    float totalWidth = std::min(numPlayers, maxColumns) * (2 * circleRadius + padding) - padding;
    float totalHeight = rows * (2 * circleRadius + padding) - padding;

    // Calculate starting position to center the grid
    float startX = (viewSize.x - totalWidth) / 2.f;
    float startY = (viewSize.y - totalHeight) / 2.f;

    clickableTexts.clear();

    for (auto &pd : clientMan->getPlayerList().getPlayers())
    {
        sf::CircleShape circle(circleRadius);
        circle.setFillColor(sf::Color::Cyan);
        circle.setOutlineThickness(2.0f);
        circle.setOutlineColor(sf::Color::Black);

        // Calculate the position for this circle
        float x = startX + (index % maxColumns) * (2 * circleRadius + padding);
        float y = startY + (index / maxColumns) * (2 * circleRadius + padding);

        // Set circle position
        circle.setPosition(x, y);
        window.draw(circle);

        // Create and position the text (centered horizontally below the circle)
        Button itext(
            defaultFont,
            pd.playerName + " " + std::to_string(pd.id),
            sf::Color::Red,
            {x - circleRadius / 2, y + 2 * circleRadius + 5}, 20); // Adjusted text position

        window.draw(itext);
        itext.setOnClick([&]()
                         { std::cout << " clicked " << pd.id << std::endl; });

        clickableTexts.emplace_back(itext);

        index++;
    }

    // Render static elements
    window.draw(*startGameText);
    window.draw(*mainText);
}

void LobbyScreen::initializeButtons()
{
    mainText = std::make_shared<Button>(defaultFont, "Tetris Lobby", sf::Color::Blue, sf::Vector2f{window.getSize().x / 2.f, 70.f}, 60);
    startGameText = std::make_shared<Button>(defaultFont, "Start Game", sf::Color::Green, sf::Vector2f{window.getSize().x / 2.f, 440.f}, 50);
    updateSize(window.getSize());
}

void LobbyScreen::updateSize(const sf::Vector2u a)
{
    startGameText->setOrigin(startGameText->getLocalBounds().width / 2.f, startGameText->getLocalBounds().height / 2.f);
    mainText->setOrigin(mainText->getLocalBounds().width / 2.f, mainText->getLocalBounds().height / 2.f);
}
