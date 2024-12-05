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
#include "InteractiveText.hpp"

class MenuScreen : public Screen
{
    sf::Font font;
    InteractiveText titleText;
    InteractiveText playText;
    InteractiveText quitText;
    ScreenManager &manager; // Reference to ScreenManager

public:
    MenuScreen(ScreenManager &screenManager)
        : titleText(font, "Main Menu", 40, sf::Color::White, {200, 50}),
          playText(font, "Play", 30, sf::Color::White, {200, 150}),
          quitText(font, "Quit", 30, sf::Color::White, {200, 200}),
          manager(screenManager) // Initialize manager reference
    {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"))
        {
            std::cerr << "Error loading font!" << std::endl;
        }

        playText.setOnClick([this]() {
            std::cout << "Play button clicked! Switching to Game Screen.\n";
            manager.setActiveScreen("game"); // Use ScreenManager to switch screens
        });

        quitText.setOnClick([this]() {
            std::cout << "Quit button clicked!\n";
            //manager.quit(); // Example: add a quit method to ScreenManager
        });
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        playText.handleEvent(event);
        quitText.handleEvent(event);
    }

    void update(float deltaTime) override
    {
        // Menu update logic
    }

    void render(sf::RenderWindow &window) override
    {
        titleText.render(window);
        playText.render(window);
        quitText.render(window);
    }
};
