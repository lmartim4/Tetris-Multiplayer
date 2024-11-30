#ifndef CELL_HPP
#define CELL_HPP

#include <SFML/Graphics.hpp>

enum CELL_STATES
{
    CELL_FALLED = -1,
    CELL_EMPTY = 0,
    CELL_FALLING = 1
};

class Cell
{
private:
    int state;       // 0 = vazio, -1 = fixado, 1 = em queda
    sf::Color color; // Cor do bloco
    sf::RectangleShape cell;
    int cellSize;
    int x;
    int y;

    /*
    void setPosition(){
        cell.setPosition(y * cellSize,x* cellSize);
    }
    */
public:
    Cell(){}

    // Construtor padrão
    Cell(int cellSize, int x, int y) : cellSize(cellSize), x(x), y(y)
    {
        cell = sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
        cell.setOutlineThickness(1);
        cell.setPosition(y * cellSize, x * cellSize);

        setEmpty();
    }

    // Meio nojento ter q fazer isso
    sf::RectangleShape getCell() const
    {
        return cell;
    }

    // Getter para a cor
    sf::Color getColor() const { return color; }

    // Setter para a cor
    void setColor(const sf::Color &newColor) { color = newColor; }

    // Ao inves de fazer isso

    // int getState() const { return state; } // Getter para o estado

    // Fazer isso

    // Métodos auxiliares para verificar o estado
    bool isEmpty() const { return state == CELL_EMPTY; }
    bool isFalling() const { return state == CELL_FALLING; }
    bool isFixed() const { return state == CELL_FALLED; }

    // Ao inves disso

    // void setState(int newState) { state = newState; } // Setter para o estado

    // Fazer isso

    // Define como vazio
    void setEmpty()
    {
        state = CELL_EMPTY;
        color = sf::Color::Black;
        cell.setFillColor(color);
        cell.setOutlineColor(sf::Color::White);
        // setPosition();
    }

    // Define como "em queda"
    void setFalling(const sf::Color &newColor)
    {
        state = CELL_FALLING;
        color = newColor;
        cell.setFillColor(color);
        cell.setOutlineColor(sf::Color::Black);
        // setPosition();
    }

    // Parece q tem q passar a cor, pois o estado anterior é empty
    // Define como "fixado" na pilha
    void setFixed(const sf::Color &newColor)
    {
        state = CELL_FALLED;
        color = newColor;
        cell.setFillColor(color);
        cell.setOutlineColor(sf::Color::Black);
        // setPosition();
    }
};

#endif
