#ifndef BASE_TETROMINO
#define BASE_TETROMINO

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum MOVES
{
    EMPTY,
    LEFT,
    RIGHT,
    DOWN_FASTER,
    ROT_LEFT,
    ROT_RIGHT
};

class Tetromino
{
protected:
    std::vector<std::vector<int>> shape;
    int x, y;
    sf::Color color;
    int lastMove = EMPTY;
    bool gravity = false;

public:
    static const std::vector<sf::Color> tetromino_colors;
    static const std::vector<char> types;

    // Default constructor & destructor
    Tetromino(int x0, int y0, sf::Color color) : x(x0), y(y0), color(color) {}
    virtual ~Tetromino() = default;

    int getX() const;

    int getY() const;

    // Rendering
    sf::Color getColor() const;

    int getLastMove() const;

    bool getGravity() const;

    const std::vector<std::vector<int>> &getShape() const;

    void evolveStates(bool forward);

    void updateStates();

    void action(MOVES action);

    // Fall (gravity)
    void dropGravity();
};



#endif