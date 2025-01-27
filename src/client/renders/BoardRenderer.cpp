#include "renders/BoardRenderer.hpp"

void BoardRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::lock_guard<std::mutex> lock(boardMutex);

    if (grid.size() == 0)
        return;

    for (std::vector<std::shared_ptr<CellRenderer>> row : grid)
        for (std::shared_ptr<CellRenderer> renderCell : row)
        {
            renderCell->updateData();

            if (renderCell->getCell()->getOwnerID() == client->getMyID() && renderCell->getCell()->getState() == CellState::FALLING)
                renderCell->setOutlineColor(sf::Color(255, 255, 255));
            else
                renderCell->setOutlineColor(sf::Color::Black);

            target.draw(*renderCell);
        }
}

void BoardRenderer::initializeShapes(const sf::Vector2u a)
{
    std::lock_guard<std::mutex> lock(boardMutex);

    float CELL_SIZE = computeCellSize(a);
    grid.clear();

    for (int x = 0; x < board->getHeight(); ++x)
    {
        std::vector<std::shared_ptr<CellRenderer>> row;
        for (int y = 0; y < board->getWidth(); ++y)
        {
            auto cell = std::make_shared<CellRenderer>(sf::Vector2f(0, 0), sf::Vector2f(0, 0), CellRenderMode::VerticalGradient, board->getGrid()[x][y]);
            row.push_back(cell);
        }
        grid.push_back(row);
    }
}

void BoardRenderer::updateSize(const sf::Vector2u a)
{
    std::lock_guard<std::mutex> lock(boardMutex);
    // std::cout << " a = " << a.x << ", " << a.y << std::endl;
    float CELL_SIZE = computeCellSize(a);

    for (int x = 0; x < board->getHeight(); x++)
        for (int y = 0; y < board->getWidth(); y++)
            grid[x][y]->refreshPosition(sf::Vector2f(CELL_SIZE - 10.f, CELL_SIZE - 10.f), sf::Vector2f(y * CELL_SIZE, x * CELL_SIZE));
}

BoardRenderer::BoardRenderer(std::shared_ptr<ClientManager> client, std::shared_ptr<TetrisBoard> tetrisBoard, const sf::Vector2u initialSize)
    : board(tetrisBoard), client(client)
{
    initializeShapes(initialSize);
}

float BoardRenderer::computeCellSize(const sf::Vector2u a) const
{
    int boardWidth = board->getWidth();
    int boardHeight = board->getHeight();

    if (boardWidth <= 0 || boardHeight <= 0)
        return 15;

    float maxBoardWidth = a.x * 0.8;
    float maxBoardHeight = a.y;

    float cellSizeByWidth = maxBoardWidth / boardWidth;
    float cellSizeByHeight = maxBoardHeight / boardHeight;

    float computedCellSize = std::min(cellSizeByWidth, cellSizeByHeight);

    return computedCellSize;
}

void BoardRenderer::updateBoardFromJson(const nlohmann::json &boardData, sf::RenderTarget &target)
{
    board->deserialize(boardData);

    if (board->getHeight() != grid.size() || (board->getWidth() != grid[0].size()))
    {
        initializeShapes(target.getSize());
        updateSize(target.getSize());
    }
}