#include "MenuScreen.hpp"

MenuScreen::MenuScreen(sf::RenderWindow &window, ScreenManager &screenManager, std::shared_ptr<ClientManager> clientManager)
    : Screen(window), manager(screenManager), clientMan(clientManager)
{
    initializeButtons();

    TextureLoader::getInstance().registerAtlas("game_atlas", "../assets/images/lego_tetris.jpg");
    sf::IntRect tileRect(85, 80, 80 * 5, 80 * 5);
    setBackground("game_atlas", "background_tile", tileRect);

    connectText->setOnClick([this]()
                            {
                                manager.setActiveScreen("waiting-connection");
                                clientMan->connect(); });

    quitText->setOnClick([this]()
                         { manager.quit(); });
}

void MenuScreen::initializeButtons()
{

    // Define offsets if needed
    const float x_offset_screen = 150.f; // Adjust as necessary
    const float y_offset_screen = 120.f; // Adjust as necessary

    sf::Color main_menu_color = {81, 255, 23};
    sf::Color button_color = {255, 213, 0};

    // Initialize titleText
    titleText = std::make_shared<Button>(
        defaultFont,
        "Main Menu",
        main_menu_color,
        sf::Vector2f(x_offset_screen + 200.f, y_offset_screen + 50.f),
        100);

    titleText->setOutlineColor(sf::Color::Black);
    titleText->setOutlineThickness(4.0f);

    // Initialize connectText
    connectText = std::make_shared<Button>(
        defaultFont,
        "Connect",
        button_color,
        sf::Vector2f(x_offset_screen + 330.f, y_offset_screen + 200.f),
        40);

    connectText->setOutlineColor(sf::Color::Black);
    connectText->setOutlineThickness(4.0f);

    // Initialize quitText
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
    
    titleText->render(window);
    connectText->render(window);
    quitText->render(window);
}

void MenuScreen::updateSize(const sf::Vector2u a)
{
    // Get the new window dimensions
    float windowWidth = static_cast<float>(a.x);
    float windowHeight = static_cast<float>(a.y);

    // Calculate the center of the window
    sf::Vector2f center(windowWidth / 2.f, windowHeight / 2.f);

    // Position the title text above the center
    // Assuming the title text height is around 50 pixels
    titleText->setPosition(center.x, center.y - 150.f); // Adjust the Y offset as needed

    // Position the connect and quit texts below the center
    connectText->setPosition(center.x, center.y);      // Directly at the center or adjust as needed
    quitText->setPosition(center.x, center.y + 100.f); // Adjust the Y offset as needed

    // Optionally, center the text horizontally by adjusting the origin
    // This ensures that the text is perfectly centered based on its width
    titleText->setOrigin(titleText->getLocalBounds().width / 2.f, titleText->getLocalBounds().height / 2.f);
    connectText->setOrigin(connectText->getLocalBounds().width / 2.f, connectText->getLocalBounds().height / 2.f);
    quitText->setOrigin(quitText->getLocalBounds().width / 2.f, quitText->getLocalBounds().height / 2.f);
}
