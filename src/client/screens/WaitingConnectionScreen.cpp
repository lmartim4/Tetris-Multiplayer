#include "WaitingConnectionScreen.hpp"

WaitingConnectionScreen::WaitingConnectionScreen(sf::RenderWindow &window, ScreenManager &screenManager, std::shared_ptr<ClientManager> clientManager)
    : Screen(window),
      titleText(defaultFont, "Waiting for server...", sf::Color::Black, {(window.getSize().x / 2.f), (window.getSize().y / 2.f - 50.f)}, 60),
      backText(defaultFont, "Back", sf::Color::Black, {(window.getSize().x / 2.f), (window.getSize().y / 2.f + 100.f)}, 40),
      screenMan(screenManager), clientMan(clientManager)
{
    titleText.setOutlineColor(sf::Color::White);
    titleText.setOutlineThickness(2.f);
    backText.setOutlineColor(sf::Color::White);
    backText.setOutlineThickness(2.f);

    setBackground("espera_connec", "../assets/images/cthomas_garnier_tga_7836.jpg");
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
    renderBackground();
    window.draw(titleText);
    window.draw(backText);
}

void WaitingConnectionScreen::updateSize(const sf::Vector2u a)
{
    resizeBackgound(a);
    backText.setOrigin(backText.getLocalBounds().width / 2.f, backText.getLocalBounds().height / 2.f);
    titleText.setOrigin(titleText.getLocalBounds().width / 2.f, titleText.getLocalBounds().height / 2.f);
}
