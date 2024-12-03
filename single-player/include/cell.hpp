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
    int state;               // 0 = vazio, -1 = fixado, 1 = em queda
    sf::Color color;         // Cor do bloco
    sf::RectangleShape cell; // Forma gráfica da célula
    int cellSize;            // Tamanho da célula
    int x;                   // Linha (posição X)
    int y;                   // Coluna (posição Y)

    // Define a posição da célula
    void setPosition();

public:
    // Construtor padrão
    Cell();

    // Construtor parametrizado
    Cell(int cellSize, int x, int y);

    // Getter para a forma gráfica da célula
    sf::RectangleShape getCell() const;

    // Getter para a cor
    sf::Color getColor() const;

    // Setter para a cor
    void setColor(const sf::Color &newColor);

    // Verificadores de estado
    bool isEmpty() const;
    bool isFalling() const;
    bool isFixed() const;

    // Setters de estado
    void setEmpty();
    void setFalling(const sf::Color &newColor);
    void setFixed(const sf::Color &newColor);
};

#endif
