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

class Screen
{
private:
    sf::Texture *backgroundTexture = nullptr;
    sf::Sprite backgroundSprite;

public:
    Screen(sf::RenderWindow &window);
    virtual ~Screen() {}

    void stop();

    virtual void handleEvent(sf::Event event, class ScreenManager &manager) = 0;
    virtual void update(float deltaTime) = 0;

    virtual void render(sf::RenderWindow &window) = 0;

    sf::Font &getDefaultFont() { return defaultFont; }

protected:
    std::atomic<bool> running{true};
    sf::Font defaultFont;
    sf::RenderWindow &window;

    void setBackground(const std::string &textureName, const std::string &filePath)
    {
        backgroundTexture = &TextureLoader::getInstance().getTexture(textureName, filePath);
        backgroundSprite.setTexture(*backgroundTexture);
    }

    void renderBackground()
    {
        if (backgroundTexture)
        {
            window.draw(backgroundSprite);
        }
    }
};