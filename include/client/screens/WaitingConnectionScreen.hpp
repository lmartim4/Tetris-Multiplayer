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

class WaitingConnectionScreen : public Screen
{
    InteractiveText titleText;
    InteractiveText backText;

    ScreenManager &screenMan;
    ClientManager &clientMan;

#define x_offset_screen 80
#define y_offset_screen 80

public:
    WaitingConnectionScreen(ScreenManager &screenManager, ClientManager &clientManager)
        : titleText(defaultFont, "Waiting for server...", 40, sf::Color::White, {x_offset_screen + 200, y_offset_screen + 50}),
          backText(defaultFont, "Back", 30, sf::Color::White, {x_offset_screen + 300, y_offset_screen + 200}),
          screenMan(screenManager), clientMan(clientManager)
    {
        backText.setOnClick([this]()
                            {
                                std::cout << "Getting back to main screen\n";
                                clientMan.disconnect();
                                screenMan.setActiveScreen("main-menu"); });
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        backText.handleEvent(event);
    }

    void update(float deltaTime) override
    {
        if (clientMan.IsConnected())
        {
            screenMan.setActiveScreen("lobby");
        }
    }

    void render(sf::RenderWindow &window) override
    {
        titleText.render(window);
        backText.render(window);
    }
};
