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

constexpr float x_offset_screen = 80;
constexpr float y_offset_screen = 120;

class MenuScreen : public Screen
{
    InteractiveText titleText;
    InteractiveText connectText;
    InteractiveText quitText;
    ScreenManager &manager; // Reference to ScreenManager
    ClientManager &clientMan;

public:
    MenuScreen(ScreenManager &screenManager, ClientManager &clientManager)
        : titleText(defaultFont, "Main Menu", sf::Color::White, {x_offset_screen + 200, y_offset_screen + 50}, 40),
          connectText(defaultFont, "Connect", sf::Color::White, {x_offset_screen + 330, y_offset_screen + 200}, 25),
          quitText(defaultFont, "Quit", sf::Color::White, {x_offset_screen + 200, y_offset_screen + 200}, 25),
          manager(screenManager), clientMan(clientManager)
    {
        connectText.setOnClick([this]()
                               {
                                manager.setActiveScreen("waiting-connection");
                                clientMan.connect(); });

        quitText.setOnClick([this]()
                            { manager.quit(); });
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
