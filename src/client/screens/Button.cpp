#include "Button.hpp"

Button::Button(const sf::Font &font, const std::string &str, sf::Color color, sf::Vector2f position, int fontSize)
{
    setFont(font);
    setString(str);
    setCharacterSize(fontSize);
    setFillColor(color);
    setPosition(position);
}

void Button::setOnClick(const std::function<void()> &callback) { onClickCallback = callback; }

void Button::handleEvent(const sf::Event &event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

        if (getGlobalBounds().contains(mousePos))
            if (onClickCallback)
                onClickCallback();
    }
}

sf::FloatRect Button::getBounds() const
{
    sf::FloatRect textBounds = getGlobalBounds();
    sf::Vector2f position = getPosition();

    return sf::FloatRect(position.x, position.y, textBounds.width, textBounds.height);
}
