#pragma once

#include <SFML/Graphics.hpp>

class Resizeable
{
private:
protected:
    virtual void updateSize(const sf::Vector2u a) = 0;

public:
    void onResize(sf::Event::SizeEvent e)
    {
        updateSize(sf::Vector2u{e.width, e.height});
    }

    Resizeable() = default;
    ~Resizeable() = default;
};
