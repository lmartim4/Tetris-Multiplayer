#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <mutex>

#include "Tetromino.hpp"
#include "CellRenderer.hpp"
#include "Cell.hpp"

class MiniBoard : public sf::Drawable
{
private:
    mutable std::mutex renderMutex;

    sf::Vector2f position;                                              // Top-left position of the MiniBoard
    float cellSize;                                                     // Size of each cell
    std::vector<std::vector<std::shared_ptr<CellRenderer>>> renderGrid; // Grid of cell renderers

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    MiniBoard(sf::Vector2f position, float cellSize);

    void setTetromino(const Tetromino &tetromino, CellRenderMode renderMode);

    void refreshPosition(const sf::RenderWindow &window);

    float getWidth() const;

    float getHeight() const;

    sf::Vector2f getPosition() const;
};
