#include "Screen.hpp"

Screen::Screen()
{
    if (!defaultFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"))
    {
        std::cerr << "Error loading font!" << std::endl;
    }
}

void Screen::stop()
{
    running = false;
}