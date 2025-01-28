#include "MenuScreen.hpp"

MenuScreen::MenuScreen(sf::RenderWindow &window, ScreenManager &screenManager, std::shared_ptr<ClientManager> clientManager)
    : Screen(window), manager(screenManager), clientMan(clientManager)
{
    initializeButtons();
    setBackground("menu-principal", "../assets/images/folha.jpg");

    connectText->setOnClick([this]()
                            {
                                manager.setActiveScreen("waiting-connection");
                                clientMan->connect(); });

    quitText->setOnClick([this]()
                         { manager.quit(); });
}

void MenuScreen::initializeButtons()
{
    const float x_offset_screen = 180.f;
    const float y_offset_screen = 150.f;

    sf::Color main_menu_color = {81, 255, 23};
    sf::Color button_color = {255, 213, 0};

    titleText = std::make_shared<Button>(
        defaultFont,
        "Main Menu",
        main_menu_color,
        sf::Vector2f(x_offset_screen + 270.f, y_offset_screen + 50.f),
        100);

    titleText->setOutlineColor(sf::Color::Black);
    titleText->setOutlineThickness(4.0f);

    connectText = std::make_shared<Button>(
        defaultFont,
        "Connect",
        button_color,
        sf::Vector2f(x_offset_screen + 330.f, y_offset_screen + 200.f),
        40);

    connectText->setOutlineColor(sf::Color::Black);
    connectText->setOutlineThickness(4.0f);

    quitText = std::make_shared<Button>(
        defaultFont,
        "Quit",
        button_color,
        sf::Vector2f(x_offset_screen + 200.f, y_offset_screen + 200.f),
        40);

    quitText->setOutlineColor(sf::Color::Black);
    quitText->setOutlineThickness(4.0f);
}

void MenuScreen::handleEvent(sf::Event event, ScreenManager &manager)
{

    connectText->handleEvent(event, manager.getWindow());
    quitText->handleEvent(event, manager.getWindow());
}

void MenuScreen::update(float deltaTime)
{
    // Menu update logic
}

void MenuScreen::render(sf::RenderWindow &window)
{

    renderBackground();

    window.draw(*titleText);
    window.draw(*connectText);
    window.draw(*quitText);
}

void MenuScreen::updateSize(const sf::Vector2u a)
{
    titleText->setOrigin(titleText->getLocalBounds().width / 2.f, titleText->getLocalBounds().height / 2.f);
    connectText->setOrigin(connectText->getLocalBounds().width / 2.f, connectText->getLocalBounds().height / 2.f);
    quitText->setOrigin(quitText->getLocalBounds().width / 2.f, quitText->getLocalBounds().height / 2.f);
}
