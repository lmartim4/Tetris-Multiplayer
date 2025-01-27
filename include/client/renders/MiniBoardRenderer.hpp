#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <mutex>

#include "Tetromino.hpp"
#include "CellRenderer.hpp"
#include "Cell.hpp"
#include "Resizeable.hpp"
class MiniBoardRenderer : public sf::Drawable, public Resizeable
{
private:
    mutable std::mutex renderMutex;

    float cellSize;                                                     // Size of each cell
    sf::Vector2f position;                                              // Top-left position of the MiniBoard
    std::vector<std::vector<std::shared_ptr<CellRenderer>>> renderGrid; // Grid of cell renderers
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    MiniBoardRenderer(sf::Vector2f position, float cellSize);
    
    void updateSize(const sf::Vector2u) override;
    void setTetromino(std::shared_ptr<Tetromino> t, CellRenderMode renderMode);
    float getWidth() const;
    float getHeight() const;

    sf::Vector2f getPosition() const;
};
