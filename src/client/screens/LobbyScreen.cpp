#include "LobbyScreen.hpp"

LobbyScreen::LobbyScreen(sf::RenderWindow &window, ClientManager &clientManager)
    : Screen(window),
      clientMan(clientManager),
      mainText(defaultFont, "Tetris Lobby", sf::Color::Blue, {230, 20}, 50),
      startGameText(defaultFont, "Start Game", sf::Color::Green, {230, 380}, 40)
{
    startGameText.setOnClick([&clientManager]()
                             { clientManager.request_game_start(); });
}

void LobbyScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    startGameText.handleEvent(event, manager.getWindow());

    for (Button ct : clickableTexts)
        ct.handleEvent(event, manager.getWindow());

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        std::cout << "Lobby event: escape pressed" << std::endl;
}

void LobbyScreen::update(float deltaTime) {}

void LobbyScreen::render(sf::RenderWindow &window)
{
    startGameText.render(window);
    mainText.render(window);

    int index = 0;
    int maxColumns = 5; // Max number of circles per row
    int numPlayers = clientMan.getPlayerList().getPlayers().size();

    // Calculate the number of rows needed
    int rows = (numPlayers + maxColumns - 1) / maxColumns;

    // Calculate the total width and height of the grid
    float totalWidth = maxColumns * (2 * circleRadius + padding) - padding;
    float totalHeight = rows * (2 * circleRadius + padding) - padding;

    // Get the size of the *active view*, not raw window pixels
    sf::Vector2f viewSize = window.getView().getSize();
    float startX = (viewSize.x - totalWidth) / 2.f;
    float startY = (viewSize.y - totalHeight) / 2.f;

    clickableTexts.clear();

    for (auto &pd : clientMan.getPlayerList().getPlayers())
    {
        sf::CircleShape circle(circleRadius);

        circle.setFillColor(sf::Color::Cyan);
        circle.setOutlineThickness(2.0f);
        circle.setOutlineColor(sf::Color::Black);

        // Calculate position for the circle
        float x = startX + (index % maxColumns) * (2 * circleRadius + padding);
        float y = startY + (index / maxColumns) * (2 * circleRadius + padding);

        // Set circle position
        circle.setPosition(x, y);
        window.draw(circle);

        // Create and position the text
        Button itext(
            defaultFont,
            pd.playerName + " " + std::to_string(pd.id),
            sf::Color::Red,
            {x - circleRadius, y + 2 * circleRadius}, 20); // Centered text below circle

        itext.render(window);
        itext.setOnClick([&]()
                         { std::cout << " clicked " << pd.id << std::endl; });

        clickableTexts.emplace_back(itext);

        index++;
    }
}