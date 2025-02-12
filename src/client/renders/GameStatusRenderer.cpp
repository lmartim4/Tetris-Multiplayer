#include "GameStatusRenderer.hpp"

void GameStatusRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(level);
    target.draw(lines);
    target.draw(score);
}

void GameStatusRenderer::updateGameStatus(GameStatus data)
{
    level.setString("Level: " + std::to_string(data.getLevel()));
    lines.setString("Lines: " + std::to_string(data.getTotalLinesCleared()));
    score.setString("Score: " + std::to_string(data.getScore()));
}

void GameStatusRenderer::updateSize(const sf::Vector2u a)
{
    float w = static_cast<float>(a.x);
    float h = static_cast<float>(a.y);

    // Margins
    float marginLeft = 220.f;  // distance from the left of the right region
    float marginRight = 150.f; // distance from the right edge of the window

    // The "right side" region is from 80% of the width up to the window’s right edge
    float regionLeft = w - marginLeft;
    float regionRight = w - marginRight;

    // Bottom margin start (for stacking the texts vertically upward)
    float marginBottom = h - 170.f;

    // Example: place them in increments of 50px
    lines.setPosition(regionLeft, marginBottom);
    level.setPosition(regionLeft, marginBottom + 50.f);
    score.setPosition(regionLeft, marginBottom + 100.f);
}

GameStatusRenderer::GameStatusRenderer(sf::Font &defaultFont)
    : score(defaultFont, "Score: 0", sf::Color::White, sf::Vector2f(400, 400), 40),
      lines(defaultFont, "Lines: 0", sf::Color::White, sf::Vector2f(400, 350), 40),
      level(defaultFont, "Level: 0", sf::Color::White, sf::Vector2f(400, 250), 40)
{

    score.setOutlineColor(sf::Color::Black);
    score.setOutlineThickness(2.f);

    lines.setOutlineColor(sf::Color::Black);
    lines.setOutlineThickness(2.f);

    level.setOutlineColor(sf::Color::Black);
    level.setOutlineThickness(2.f);
}
