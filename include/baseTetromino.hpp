#ifndef BASE_TETROMINO
#define BASE_TETROMINO

#include <string>
#include <vector>

class Tetromino {
protected:
    std::vector<std::vector<int>> shape;
    int x, y;
    char type;
    std::string color;  // Cor do tetromino
    
    // Tirar o atributo "type"
public:
    // Default constructor & destructor
    Tetromino(char type, std::string color) : type(type), x(0), y(0), color(color) {}
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

    std::vector<std::vector<int>> getShape(){
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
    std::string getColor() const { return color; }
};

#endif