#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <mutex>

#include "TetrisBoard.hpp"
#include "CellRenderer.hpp"
#include "ClientManager.hpp"

class BoardRenderer : public sf::Drawable
{
private:
    std::shared_ptr<ClientManager> client;
    std::shared_ptr<TetrisBoard> board;
    std::vector<std::vector<std::shared_ptr<CellRenderer>>> grid;

    mutable std::mutex boardMutex;

    float computeCellSize(sf::RenderTarget &target) const;

    // Override the draw function from sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    // Helper function to initialize cell shapes
    void initializeShapes(sf::RenderTarget &target);

public:
    // Constructor takes a shared pointer to TetrisBoard and cell size
    BoardRenderer(std::shared_ptr<ClientManager> client, std::shared_ptr<TetrisBoard> tetrisBoard);
    ~BoardRenderer() = default;

    void onWindowResize(sf::RenderTarget &target);
    void updateBoardFromJson(const nlohmann::json &boardData, sf::RenderTarget &target);
};
