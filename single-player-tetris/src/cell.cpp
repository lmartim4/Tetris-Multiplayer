#include "../include/cell.hpp"

// Construtor padrão
Cell::Cell() {}

// Construtor parametrizado
Cell::Cell(int cellSize, int x, int y) : cellSize(cellSize), x(x), y(y)
{
    cell = sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
    cell.setOutlineThickness(1);
    cell.setPosition(y * cellSize, x * cellSize);
    setEmpty();
}


// Getter para a forma gráfica da célula
sf::RectangleShape Cell::getCell() const
{
    return cell;
}

// Getter para a cor
sf::Color Cell::getColor() const
{
    return color;
}

// Verifica se a célula está vazia
bool Cell::isEmpty() const
{
    return state == CELL_EMPTY;
}

// Verifica se a célula está em queda
bool Cell::isFalling() const
{
    return state == CELL_FALLING;
}

// Verifica se a célula está fixa
bool Cell::isFixed() const
{
    return state == CELL_FALLED;
}

// Define o estado da célula como vazia
void Cell::setEmpty()
{
    state = CELL_EMPTY;
    color = sf::Color::Black;
    cell.setFillColor(color);
    cell.setOutlineColor(sf::Color::White);
}

// Define o estado da célula como em queda
void Cell::setFalling(const sf::Color &newColor)
{
    state = CELL_FALLING;
    color = newColor;
    cell.setFillColor(color);
    cell.setOutlineColor(sf::Color::Black);
}

// Define o estado da célula como fixa
void Cell::setFixed(const sf::Color &newColor)
{
    state = CELL_FALLED;
    color = newColor;
    cell.setFillColor(color);
    cell.setOutlineColor(sf::Color::Black);
}
