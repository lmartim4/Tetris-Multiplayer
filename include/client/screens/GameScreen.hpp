#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <map>

#include "Screen.hpp"

class GameScreen : public Screen
{
    sf::Text gameText;

public:
    GameScreen(sf::RenderWindow &window);

    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    void updateSize(const sf::Vector2u a) override;
};