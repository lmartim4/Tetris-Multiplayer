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

// Menu Screen
class MenuScreen : public Screen
{
    sf::Font font;
    sf::Text titleText, playText, quitText;

public:
    MenuScreen()
    {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"))
        {
            std::cerr << "Error loading font!" << std::endl;
        }

        titleText.setFont(font);
        titleText.setString("Main Menu");
        titleText.setCharacterSize(40);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(200, 50);

        playText.setFont(font);
        playText.setString("Play");
        playText.setCharacterSize(30);
        playText.setFillColor(sf::Color::White);
        playText.setPosition(200, 150);

        quitText.setFont(font);
        quitText.setString("Quit");
        quitText.setCharacterSize(30);
        quitText.setFillColor(sf::Color::White);
        quitText.setPosition(200, 200);
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            int mouseX = event.mouseButton.x;
            int mouseY = event.mouseButton.y;

            if (playText.getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY)))
            {
                std::cout << "Play button clicked! Switching to Game Screen.\n";
                manager.setActiveScreen("game");
            }

            if (quitText.getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY)))
            {
                std::cout << "Quit button clicked!\n";
            }
        }
    }

    void update(float deltaTime) override
    {
        // Menu update logic
        if (!running)
            return;
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    void render(sf::RenderWindow &window) override
    {
        window.draw(titleText);
        window.draw(playText);
        window.draw(quitText);
    }
};