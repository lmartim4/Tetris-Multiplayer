#include "MiniBoardRenderer.hpp"

void MiniBoardRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::lock_guard<std::mutex> lock(renderMutex); // Lock the mutex

    for (const auto &row : renderGrid)
        for (const auto &cell : row)
        {
            cell->updateData();
            target.draw(*cell, states);
        }
}

MiniBoardRenderer::MiniBoardRenderer(sf::Vector2f position, float cellSize)
    : position(position), cellSize(cellSize)
{
}

void MiniBoardRenderer::setTetromino(const Tetromino &tetromino, CellRenderMode renderMode)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    renderGrid.clear();

    const std::shared_ptr<TetrominoShape> tshape = tetromino.getShape();
    auto shape = tshape->getShape();

    int height = shape.size();
    if (height == 0)
    {
        std::cout << "Error: Tetromino shape is empty!\n";
        return;
    }

    int width = shape[0].size();

    for (int x = 0; x < height; ++x)
    {
        std::vector<std::shared_ptr<CellRenderer>> row;
        for (int y = 0; y < width; ++y)
        {
            if (shape[x][y] == 0)
                continue;

            std::shared_ptr<Cell> cell = std::make_shared<Cell>(Coordinate(x, y));
            cell->setColor(tetromino.getColor());

            auto cellRenderer = std::make_shared<CellRenderer>(
                sf::Vector2f(cellSize, cellSize),
                sf::Vector2f(position.x + y * cellSize, position.y + x * cellSize),
                renderMode,
                cell);

            row.push_back(cellRenderer);
        }

        renderGrid.push_back(row);
    }

    if (renderGrid.empty())
    {
        std::cout << "Error: Render grid is empty after processing tetromino!\n";
    }
}

void MiniBoardRenderer::refreshPosition(sf::RenderTarget &target)
{
    std::lock_guard<std::mutex> lock(renderMutex); // Lock the mutex

    position.x = target.getSize().x - 4 * cellSize - 100.f;
    position.y = 100.f;

    if (renderGrid.size() == 0)
        return;

    position.x = target.getSize().x - renderGrid[0].size() * cellSize - 100.f; // 10px margin from the right
    position.y = 100.f;                                                        // 10px margin from the top

    for (int x = 0; x < renderGrid.size(); ++x)
    {
        for (int y = 0; y < renderGrid[x].size(); ++y)
        {
            renderGrid[x][y]->refreshPosition(
                sf::Vector2f(cellSize, cellSize),
                sf::Vector2f(position.x + y * cellSize, position.y + x * cellSize));
        }
    }
}

float MiniBoardRenderer::getWidth() const
{
    if (renderGrid.empty() || renderGrid[0].empty())
        return 0.f; // Handle empty grid case

    return renderGrid[0].size() * cellSize; // Number of columns * cell size
}

float MiniBoardRenderer::getHeight() const
{
    if (renderGrid.empty())
        return 0.f; // Handle empty grid case

    return renderGrid.size() * cellSize; // Number of rows * cell size
}

sf::Vector2f MiniBoardRenderer::getPosition() const
{
    return position;
}