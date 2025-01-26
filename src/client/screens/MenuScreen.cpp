#include "MenuScreen.hpp"

MenuScreen::MenuScreen(sf::RenderWindow &window, ScreenManager &screenManager, std::shared_ptr<ClientManager> clientManager)
    : Screen(window),
      titleText(defaultFont, "Main Menu", sf::Color::White, {x_offset_screen + 200, y_offset_screen + 50}, 40),
      connectText(defaultFont, "Connect", sf::Color::White, {x_offset_screen + 330, y_offset_screen + 200}, 25),
      quitText(defaultFont, "Quit", sf::Color::White, {x_offset_screen + 200, y_offset_screen + 200}, 25),
      manager(screenManager), clientMan(clientManager)
{
    setBackground("game_bg", "../assets/images/coringa.png");

    connectText.setOnClick([this]()
                           {
                                manager.setActiveScreen("waiting-connection");
                                clientMan->connect(); });

    quitText.setOnClick([this]()
                        { manager.quit(); });
}

void MenuScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    connectText.handleEvent(event, manager.getWindow());
    quitText.handleEvent(event, manager.getWindow());
}

void MenuScreen::update(float deltaTime)
{
    // Menu update logic
}

void MenuScreen::render(sf::RenderWindow &window)
{
    renderBackground();
    
    titleText.render(window);
    connectText.render(window);
    quitText.render(window);
}