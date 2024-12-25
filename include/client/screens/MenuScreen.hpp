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

constexpr float x_offset_screen = 80;
constexpr float y_offset_screen = 120;

class MenuScreen : public Screen
{
    Button titleText;
    Button connectText;
    Button quitText;
    ScreenManager &manager;
    ClientManager &clientMan;
public:
    MenuScreen(ScreenManager &screenManager, ClientManager &clientManager);
    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;
};
