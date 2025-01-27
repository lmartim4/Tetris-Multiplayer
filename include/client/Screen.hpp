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
#include "TextureLoader.hpp"
#include "Background.hpp"
#include "Resizeable.hpp"

class Screen : public Resizeable
{
private:
    std::unique_ptr<Background> background;

public:
    Screen(sf::RenderWindow &window);
    virtual ~Screen() {}

    sf::Font &getDefaultFont() { return defaultFont; }

    void stop();

    virtual void handleEvent(sf::Event event, class ScreenManager &manager) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

protected:
    std::atomic<bool> running{true};
    sf::Font defaultFont;
    sf::RenderWindow &window;

    void setBackground(const std::string &atlasName, const std::string &tileName, const sf::IntRect &tileRect)
    {
        background = std::make_unique<Background>(window, atlasName, tileName, tileRect);
    }

    // Must render if wanted
    void renderBackground()
    {
        if (background)
            background->render(window);
    }

public:
    void updateSize(const sf::Vector2u a) override
    {
        sf::FloatRect visibleArea(0, 0, a.x, a.y);
        window.setView(sf::View(visibleArea));

        if (background)
            background->resize(window.getSize());
    }
};