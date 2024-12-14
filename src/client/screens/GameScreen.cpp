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
}

void GameScreen::update(float deltaTime)
{
}

void GameScreen::render(sf::RenderWindow &window)
{
    window.draw(gameText);
}