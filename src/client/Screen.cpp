#include "Screen.hpp"

Screen::Screen(sf::RenderWindow &window) : window(window)
{
    if (!defaultFont.loadFromFile("../assets/fonts/tetris-2-bombliss-credits-font.ttf"))
    {
        std::cerr << "Error loading font!" << std::endl;
    }
}

void Screen::stop()
{
    running = false;
}