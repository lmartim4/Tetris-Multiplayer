#pragma once

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "GameStatus.hpp"

class GameStatusRenderer : public sf::Drawable
{
private:
    Button score, lines, level;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    void updateGameStatus(GameStatus gs);
    void updateTextPositions(sf::RenderTarget &window);

    GameStatusRenderer(sf::Font &defaultFont);
    ~GameStatusRenderer() = default;
};
