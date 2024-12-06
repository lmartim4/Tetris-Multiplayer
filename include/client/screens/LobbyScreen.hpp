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
    std::vector<sf::CircleShape> playerCircles;
    std::map<int, std::string> players; // Map of player IDs and their names (or other details)
    const float circleRadius = 30.0f;   // Radius of each player circle
    const float padding = 10.0f;        // Padding between circles

public:
    LobbyScreen()
    {
        // Set lobby header text
        lobbyText.setFont(defaultFont);
        lobbyText.setString("Lobby Screen");
        lobbyText.setCharacterSize(40);
        lobbyText.setFillColor(sf::Color::Blue);
        lobbyText.setPosition(230, 20);
    }

    // Function to add a new player
    void addPlayer(int playerId, const std::string &playerName)
    {
        players[playerId] = playerName;

        sf::CircleShape circle(circleRadius);
        circle.setFillColor(sf::Color::Cyan);
        circle.setOutlineThickness(2.0f);
        circle.setOutlineColor(sf::Color::Black);

        // Calculate position based on the current number of players
        int index = playerCircles.size();
        float x = (index % 5) * (2 * circleRadius + padding) + 100; // Adjust columns
        float y = (index / 5) * (2 * circleRadius + padding) + 100; // Adjust rows

        circle.setPosition(x, y);
        playerCircles.push_back(circle);
    }

    // Function to remove a player
    void removePlayer(int playerId)
    {
        if (players.erase(playerId) > 0)
        {
            playerCircles.clear();
            int index = 0;
            for (const auto &[id, name] : players)
            {
                sf::CircleShape circle(circleRadius);
                circle.setFillColor(sf::Color::Cyan);
                circle.setOutlineThickness(2.0f);
                circle.setOutlineColor(sf::Color::Black);

                float x = (index % 5) * (2 * circleRadius + padding) + 100;
                float y = (index / 5) * (2 * circleRadius + padding) + 100;

                circle.setPosition(x, y);
                playerCircles.push_back(circle);
                ++index;
            }
        }
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        // Example: Handle "Escape" key to leave the lobby
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            std::cout << "Lobby event: escape pressed" << std::endl;
        }
    }

    void update(float deltaTime) override
    {
        // Update logic for lobby screen (e.g., fetching player list from server)
    }

    void render(sf::RenderWindow &window) override
    {
        // Draw the lobby text
        window.draw(lobbyText);

        // Draw all player circles
        for (auto &circle : playerCircles)
        {
            window.draw(circle);
        }
    }
};
