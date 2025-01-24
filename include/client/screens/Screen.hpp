#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <map>
#include "ScreenManager.hpp"
class Screen
{

public:
    Screen(sf::RenderWindow &window);
    void stop();
    virtual ~Screen() {}

    virtual void handleEvent(sf::Event event, class ScreenManager &manager) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

protected:
    std::atomic<bool> running{true};
    sf::Font defaultFont;
    sf::RenderWindow &window;
};