#include "GameScreen.hpp"

GameScreen::GameScreen()
{
    gameText.setFont(defaultFont);
    gameText.setString("Game Screen");
    gameText.setCharacterSize(40);
    gameText.setFillColor(sf::Color::Green);
    gameText.setPosition(200, 200);
}

void GameScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        std::cout << "Game event: escape pressed" << std::endl;
}

void GameScreen::update(float deltaTime)
{
}

void GameScreen::render(sf::RenderWindow &window)
{
    window.draw(gameText);
}