#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Button : public sf::Text
{
    std::function<void()> onClickCallback;

public:
    Button(const sf::Font &font, const std::string &str, sf::Color color, sf::Vector2f position, int fontSize);
    void setOnClick(const std::function<void()> &callback);
    void handleEvent(const sf::Event &event, sf::RenderWindow &window);
    sf::FloatRect getBounds() const;
};
