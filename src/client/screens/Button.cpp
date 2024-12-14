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

void Button::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonPressed)
        if (getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
            if (onClickCallback)
                onClickCallback();
}

void Button::render(sf::RenderWindow &window) { window.draw(*this); }