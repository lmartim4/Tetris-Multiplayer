#include "WaitingConnectionScreen.hpp"

WaitingConnectionScreen::WaitingConnectionScreen(sf::RenderWindow &window, ScreenManager &screenManager, std::shared_ptr<ClientManager> clientManager)
    : Screen(window),
      titleText(defaultFont, "Waiting for server...", sf::Color::White, {(x_offset_screen + 100.0f), (y_offset_screen + 50.0f)}, 40),
      backText(defaultFont, "Back", sf::Color::White, {(x_offset_screen + 300.0f), (y_offset_screen + 170.0f)}, 25),
      screenMan(screenManager), clientMan(clientManager)
{
    backText.setOnClick([this]()
                        {   std::cout << "Getting back to main screen\n";
                                screenMan.setActiveScreen("main-menu");
                                clientMan->disconnect(); });
}

void WaitingConnectionScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    backText.handleEvent(event, manager.getWindow());
}

void WaitingConnectionScreen::update(float deltaTime)
{
    if (clientMan->IsConnected())
        screenMan.setActiveScreen("lobby");
}

void WaitingConnectionScreen::render(sf::RenderWindow &window)
{
    titleText.render(window);
    backText.render(window);
}

void WaitingConnectionScreen::updateSize(const sf::Vector2u a)
{
}
