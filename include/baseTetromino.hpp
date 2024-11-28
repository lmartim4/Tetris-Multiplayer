#ifndef BASE_TETROMINO
#define BASE_TETROMINO

#include<SFML/Graphics.hpp>
#include <string>
#include <vector>

class Tetromino {
protected:
    std::vector<std::vector<int>> shape;
    int x, y;
    char type;
    sf::Color color;
    
    // Tirar o atributo "type"
public:
    // Default constructor & destructor
    Tetromino(char type, int x0, int y0, sf::Color color) :  x(x0), y(y0), type(type), color(color) {}
    virtual ~Tetromino() = default;

    int getX() const {
        return x;
    }

    int getY() const {     
        return y;
    }

    char getType(){
        return type;
    }

    // Rendering
    sf::Color getColor() const{
        return color;
    }

    // The function itself cannot modify the class (shape remains
    // untoched within getShape()) and the returned 
    // reference cannot be used to modify the shape externally
    const std::vector<std::vector<int>> & getShape() const{
        return shape;
    }

    // Rotations 
    void rotateLeft();
    void rotateRight();
    
    // Translations
    void moveLeft();
    void moveRight();
    
    // Fall (gravity)
    void dropSlow();
    void dropFast();
    
};

#endif