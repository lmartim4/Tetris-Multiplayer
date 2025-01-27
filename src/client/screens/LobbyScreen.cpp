#include "LobbyScreen.hpp"

LobbyScreen::LobbyScreen(sf::RenderWindow &window, std::shared_ptr<ClientManager> clientManager)
    : Screen(window),
      clientMan(clientManager),
      mainText(defaultFont, "Tetris Lobby", sf::Color::Blue, {window.getSize().x / 2 - 100, 20}, 50),
      startGameText(defaultFont, "Start Game", sf::Color::Green, {window.getSize().x / 2 - 100, 380}, 40)
{

    auto size = window.getSize();

    startGameText.setOnClick([clientManager]()
                             { clientManager->request_game_start(); });
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
    // Render static elements
    startGameText.render(window);
    mainText.render(window);

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
        // Create a circle for the player
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
            {x - circleRadius, y + 2 * circleRadius + 5}, 20); // Adjusted text position

        itext.render(window);
        itext.setOnClick([&]()
                         { std::cout << " clicked " << pd.id << std::endl; });

        clickableTexts.emplace_back(itext);

        index++;
    }
}

void LobbyScreen::updateSize(const sf::Vector2u a)
{
    sf::View view = window.getView();
    view.setSize(static_cast<float>(a.x), static_cast<float>(a.y));
    window.setView(view);
}
