#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class InteractiveText
{
    sf::Text text;
    std::function<void()> onClickCallback;

public:
    InteractiveText(const sf::Font &font, const std::string &str, sf::Color color, sf::Vector2f position)
    {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(str.length() * 5);
        text.setFillColor(color);
        text.setPosition(position);
    }

    void setOnClick(const std::function<void()> &callback) { onClickCallback = callback; }

    void handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::MouseButtonPressed)
            if (text.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                if (onClickCallback)
                    onClickCallback();
    }

    void render(sf::RenderWindow &window) { window.draw(text); }

    sf::Text &getText() { return text; }
};
