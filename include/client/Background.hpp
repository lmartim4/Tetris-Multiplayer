#pragma once

#include <SFML/Graphics.hpp>
#include "TextureLoader.hpp"

class Background
{
public:
    // Constructor takes window reference, atlas name, tile name, and tile rectangle
    Background(sf::RenderWindow &window, const std::string &atlasName, const std::string &tileName)
    {
        // Extract the tile texture from the atlas
        tileTexture = &TextureLoader::getInstance().getTexture(atlasName, tileName);
        
        // Set up the rectangle shape to cover the entire window
        shape.setSize(static_cast<sf::Vector2f>(window.getSize()));
        shape.setTexture(tileTexture);
        shape.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
    }

    void resize(const sf::Vector2u &newSize)
    {
        std::lock_guard lock(mtx);

        shape.setSize(static_cast<sf::Vector2f>(newSize));
        shape.setTextureRect(sf::IntRect(0, 0, newSize.x, newSize.y));
    }

    // Render the background
    void render(sf::RenderWindow &window)
    {
        std::lock_guard lock(mtx);
        window.draw(shape);
    }

private:
    sf::RectangleShape shape;
    sf::Texture *tileTexture;
    std::mutex mtx;
};
