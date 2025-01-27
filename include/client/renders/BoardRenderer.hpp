#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <mutex>

#include "TetrisBoard.hpp"
#include "CellRenderer.hpp"
#include "ClientManager.hpp"
#include "Resizeable.hpp"

class BoardRenderer : public sf::Drawable, public Resizeable
{
private:
    std::shared_ptr<ClientManager> client;
    std::shared_ptr<TetrisBoard> board;
    std::vector<std::vector<std::shared_ptr<CellRenderer>>> grid;

    mutable std::mutex boardMutex;

    void initializeShapes(const sf::Vector2u a);
    float computeCellSize(const sf::Vector2u a) const;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    BoardRenderer(std::shared_ptr<ClientManager> client, std::shared_ptr<TetrisBoard> tetrisBoard, const sf::Vector2u initialSize);
    ~BoardRenderer() = default;
    void updateBoardFromJson(const nlohmann::json &boardData, sf::RenderTarget &target);
    void updateSize(const sf::Vector2u a) override;
};
