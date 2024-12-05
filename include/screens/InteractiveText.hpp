#include <SFML/Graphics.hpp>
#include <functional>

class InteractiveText {
    sf::Text text;
    std::function<void()> onClickCallback;

public:
    InteractiveText(const sf::Font &font, const std::string &str, unsigned int charSize, sf::Color color, sf::Vector2f position)
    {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(charSize);
        text.setFillColor(color);
        text.setPosition(position);
    }

    void setOnClick(const std::function<void()> &callback)
    {
        onClickCallback = callback;
    }

    void handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (text.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
            {
                if (onClickCallback)
                    onClickCallback();
            }
        }
    }

    void render(sf::RenderWindow &window)
    {
        window.draw(text);
    }

    // Provide access to sf::Text for customization
    sf::Text &getText() { return text; }
};
