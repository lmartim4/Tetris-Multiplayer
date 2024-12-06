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
    const float padding = 10.0f;

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
        for (auto &pd : clientMan.getDataPlayers())
        {
            sf::CircleShape circle(circleRadius);

            circle.setFillColor(sf::Color::Cyan);
            circle.setOutlineThickness(2.0f);
            circle.setOutlineColor(sf::Color::Black);

            float x = (index % 5) * (2 * circleRadius + padding) + 100;
            float y = (index / 5) * (2 * circleRadius + padding) + 100;

            InteractiveText itext(defaultFont, "", sf::Color::Red, {x, y});
            
            itext.render(window);
            circle.setPosition(x, y);
            window.draw(circle);
            index++;
        }
    }
};
