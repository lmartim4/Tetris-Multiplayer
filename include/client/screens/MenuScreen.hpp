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
    ScreenManager &manager;

    std::shared_ptr<ClientManager> clientMan;
    std::shared_ptr<Button> titleText, connectText, quitText;

    void initializeButtons();

public:
    MenuScreen(sf::RenderWindow &window, ScreenManager &screenManager, std::shared_ptr<ClientManager> clientManager);
    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    void updateSize(const sf::Vector2u a) override;
};
