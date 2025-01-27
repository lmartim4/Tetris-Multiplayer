#pragma once

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "GameStatus.hpp"
#include "Resizeable.hpp"

class GameStatusRenderer : public sf::Drawable, public Resizeable
{

private:
    Button score, lines, level;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    void updateSize(const sf::Vector2u a) override;
    void updateGameStatus(GameStatus gs);

    GameStatusRenderer(sf::Font &defaultFont);
    ~GameStatusRenderer() = default;
};
