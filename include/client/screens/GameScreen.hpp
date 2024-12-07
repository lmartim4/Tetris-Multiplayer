#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <map>

#include "Screen.hpp"

// Game Screen
class GameScreen : public Screen
{
    sf::Text gameText;

public:
    GameScreen()
    {
        gameText.setFont(defaultFont);
        gameText.setString("Game Screen");
        gameText.setCharacterSize(40);
        gameText.setFillColor(sf::Color::Green);
        gameText.setPosition(200, 200);
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            std::cout << "Game event: escape pressed" << std::endl;
        }
    }

    void update(float deltaTime) override
    {
    }

    void render(sf::RenderWindow &window) override
    {
        window.draw(gameText);
    }
};