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
#include "ClientManager.hpp"

class MenuScreen : public Screen
{
    InteractiveText titleText;
    InteractiveText connectText;
    InteractiveText quitText;
    ScreenManager &manager; // Reference to ScreenManager
    ClientManager &clientMan;
#define x_offset_screen 80
#define y_offset_screen 80

public:
    MenuScreen(ScreenManager &screenManager, ClientManager &clientManager)
        : titleText(defaultFont, "Main Menu", 40, sf::Color::White, {x_offset_screen + 200, y_offset_screen + 50}),
          connectText(defaultFont, "Connect", 30, sf::Color::White, {x_offset_screen + 300, y_offset_screen + 200}),
          quitText(defaultFont, "Quit", 30, sf::Color::White, {x_offset_screen + 200, y_offset_screen + 200}),
          manager(screenManager), clientMan(clientManager)
    {
        connectText.setOnClick([this]()
                               {
                                std::cout << "Trying to connect\n";
                                manager.setActiveScreen("waiting-connection");
                                clientMan.connect(); });

        quitText.setOnClick([this]()
                            {
                                std::cout << "Quit button clicked!\n";
                                // manager.quit(); // Example: add a quit method to ScreenManager
                            });
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        connectText.handleEvent(event);
        quitText.handleEvent(event);
    }

    void update(float deltaTime) override
    {
        // Menu update logic
    }

    void render(sf::RenderWindow &window) override
    {
        titleText.render(window);
        connectText.render(window);
        quitText.render(window);
    }
};
