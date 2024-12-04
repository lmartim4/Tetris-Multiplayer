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
    sf::Font font;
    sf::Text gameText;

public:
    GameScreen()
    {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"))
        {
            std::cerr << "Error loading font!" << std::endl;
        }

        gameText.setFont(font);
        gameText.setString("Game Screen");
        gameText.setCharacterSize(40);
        gameText.setFillColor(sf::Color::Green);
        gameText.setPosition(200, 200);
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        // Handle game events
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            std::cout << "Game event: escape pressed" << std::endl;
        }
    }

    void update(float deltaTime) override
    {
        // Game update logic
        if (!running)
            return;
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    void render(sf::RenderWindow &window) override
    {
        window.draw(gameText);
    }
};