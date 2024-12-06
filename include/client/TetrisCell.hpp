#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class TetrisCell
{
    sf::RectangleShape cellShape;
    std::function<void()> onClickCallback;

public:
    TetrisCell(sf::Vector2f size, sf::Vector2f position, sf::Color color)
    {
        cellShape.setSize(size);
        cellShape.setPosition(position);
        cellShape.setFillColor(color);
        cellShape.setOutlineThickness(1.0f);
        cellShape.setOutlineColor(sf::Color::Black);
    }

    void setOnClick(const std::function<void()> &callback) { onClickCallback = callback; }

    void handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::MouseButtonPressed)
            if (cellShape.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                if (onClickCallback)
                    onClickCallback();
    }

    void render(sf::RenderWindow &window) { window.draw(cellShape); }

    void setColor(const sf::Color &color) { cellShape.setFillColor(color); }

    sf::RectangleShape &getShape() { return cellShape; }
};
