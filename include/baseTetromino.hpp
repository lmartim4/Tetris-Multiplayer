#ifndef BASE_TETROMINO
#define BASE_TETROMINO

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
    Tetromino(char type, sf::Color color) :  x(0), y(0), type(type), color(color) {}
    virtual ~Tetromino() = default;

    int getX(){
        return x;
    }

    int getY(){     
        return y;
    }

    char getType(){
        return type;
    }

    // The function itself cannot modify the class (shape remains
    // untoched within getShape()) and the returned 
    // reference cannot be used to modify the shape externally
    std::vector<std::vector<int>> & getShape(){
        return shape;
    }

    // Rotations 
    virtual void rotateLeft() = 0;
    virtual void rotateRight() = 0;
    
    // Translations
    virtual void moveLeft() = 0;
    virtual void moveRight() = 0;
    
    // Fall (gravity)
    virtual void dropSlow() = 0;
    virtual void dropFast() = 0;

    // Colision detection/control & rendering
    std::vector<std::vector<int>> getShape() const { return shape; }
    
    // Rendering
    sf::Color getColor() const { return color; }
};

#endif