#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Screen.hpp"

class LobbyScreen : public Screen
{
    sf::Text lobbyText;

    ClientManager &clientMan; // Reference to the ClientManager

    const float circleRadius = 30.0f;
    const float padding = 100.0f;
    std::vector<InteractiveText> clickableTexts;

public:
    LobbyScreen(ClientManager &clientManager)
        : clientMan(clientManager)
    {
        lobbyText.setFont(defaultFont);
        lobbyText.setString("Lobby Screen");
        lobbyText.setCharacterSize(40);
        lobbyText.setFillColor(sf::Color::Blue);
        lobbyText.setPosition(230, 20);
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        for (InteractiveText ct : clickableTexts)
            ct.handleEvent(event);

        // Example: Handle "Escape" key to leave the lobby
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            std::cout << "Lobby event: escape pressed" << std::endl;
    }

    void update(float deltaTime) override
    {
        // Update logic for lobby screen (e.g., fetching player list from server)
    }

    void render(sf::RenderWindow &window) override
    {
        window.draw(lobbyText);

        int index = 0;
        int maxColumns = 5; // Max number of circles per row
        int numPlayers = clientMan.getDataPlayers().size();

        // Calculate the number of rows needed
        int rows = (numPlayers + maxColumns - 1) / maxColumns;

        // Calculate the total width and height of the grid
        float totalWidth = maxColumns * (2 * circleRadius + padding) - padding;
        float totalHeight = rows * (2 * circleRadius + padding) - padding;

        // Get window size
        sf::Vector2u windowSize = window.getSize();

        // Calculate starting position to center the grid
        float startX = (windowSize.x - totalWidth) / 2;
        float startY = (windowSize.y - totalHeight) / 2;

        clickableTexts.clear();        
        for (auto &pd : clientMan.getDataPlayers())
        {
            sf::CircleShape circle(circleRadius);

            circle.setFillColor(sf::Color::Cyan);
            circle.setOutlineThickness(2.0f);
            circle.setOutlineColor(sf::Color::Black);

            // Calculate position for the circle
            float x = startX + (index % maxColumns) * (2 * circleRadius + padding);
            float y = startY + (index / maxColumns) * (2 * circleRadius + padding);

            // Set circle position
            circle.setPosition(x, y);
            window.draw(circle);

            // Create and position the text
            InteractiveText itext(
                defaultFont,
                pd.playerName + " " + std::to_string(pd.playerID),
                sf::Color::Red,
                {x - circleRadius, y + 2 * circleRadius}, 2); // Centered text below circle

            itext.render(window);
            itext.setOnClick([&]()
                             { std::cout << " clicked " << pd.playerID << std::endl; });

            clickableTexts.emplace_back(itext);

            index++;
        }
    }
};
