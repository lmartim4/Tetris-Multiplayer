#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "CellColor.hpp"

class TetrisCell : public sf::RectangleShape
{
    std::function<void()> onClickCallback;

public:
    TetrisCell(sf::Vector2f size, sf::Vector2f position, sf::Color color)
    {
        setSize(size);
        setPosition(position);
        setFillColor(color);
        setOutlineThickness(1.0f);
        setOutlineColor(sf::Color::Black);
    }

    void setOnClick(const std::function<void()> &callback) { onClickCallback = callback; }

    void handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::MouseButtonPressed)
            if (getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                if (onClickCallback)
                    onClickCallback();
    }

    void setColor(const CellColor tc) { setFillColor(getColorFromType(tc)); }

    static sf::Color getColorFromType(CellColor type)
    {
        switch (type)
        {
        case CellColor::Empty:
            return sf::Color(50, 50, 50);
        case CellColor::Red:
            return sf::Color::Red;
        case CellColor::Green:
            return sf::Color::Green;
        case CellColor::Blue:
            return sf::Color::Blue;
        case CellColor::Yellow:
            return sf::Color::Yellow;
        default:
            return sf::Color::Magenta;
        }
    }
};
