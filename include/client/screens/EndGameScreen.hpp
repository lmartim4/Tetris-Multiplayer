#pragma once

#include "Screen.hpp"
#include "ClientManager.hpp"
#include "Button.hpp"
#include "EndGameData.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class EndGameScreen : public Screen
{
private:
    ClientManager &clientManager;
    EndGameData data;

    // Text objects
    sf::Text title;
    sf::Text totalPoints;
    sf::Text gameTime;
    sf::Text linesRemoved;
    sf::Text finalLevel;

    // For multiple players, store a vector of Text to display each player's score
    std::vector<sf::Text> playerScores;

    // Buttons
    Button quitButton;
    Button playAgainButton;

    bool hasFetchedData = false;

public:
    EndGameScreen(ClientManager &manager);
    ~EndGameScreen();

    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;
};
