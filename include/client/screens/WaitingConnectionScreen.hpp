#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <map>

#include "Screen.hpp"
#include "Button.hpp"
#include "ClientManager.hpp"

class WaitingConnectionScreen : public Screen
{
    Button titleText;
    Button backText;
    ScreenManager &screenMan;
    ClientManager &clientMan;
    static constexpr float x_offset_screen = 100.0f;
    static constexpr float y_offset_screen = 120.0f;
public:
    WaitingConnectionScreen(sf::RenderWindow &window, ScreenManager &screenManager, ClientManager &clientManager);
    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;
};
