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
    static constexpr float x_offset_screen = 100.0f;
    static constexpr float y_offset_screen = 120.0f;

public:
    WaitingConnectionScreen(ScreenManager &screenManager, ClientManager &clientManager)
        : titleText(defaultFont, "Waiting for server...", sf::Color::White, {(x_offset_screen + 100.0f), (y_offset_screen + 50.0f)}, 40),
          backText(defaultFont, "Back", sf::Color::White, {(x_offset_screen + 300.0f), (y_offset_screen + 170.0f)}, 25),
          screenMan(screenManager), clientMan(clientManager)
    {
        backText.setOnClick([this]()
                            {   std::cout << "Getting back to main screen\n";
                                screenMan.setActiveScreen("main-menu");
                                clientMan.disconnect(); 
                            });
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        backText.handleEvent(event);
    }

    void update(float deltaTime) override
    {
        if (clientMan.IsConnected())
            screenMan.setActiveScreen("lobby");
    }

    void render(sf::RenderWindow &window) override
    {
        titleText.render(window);
        backText.render(window);
    }
};
