#include "EndGameScreen.hpp"
#include <string>
#include <iostream>

float base_x = 250;
float base_y = 380;
float gap = 180;

EndGameScreen::EndGameScreen(sf::RenderWindow &window, ScreenManager &screenM, std::shared_ptr<ClientManager> clientManager)
    : Screen(window),
      screenManager(screenM),
      quitButton(defaultFont, "Quit", sf::Color::White, {base_x, base_y}, 24),
      playAgainButton(defaultFont, "Play Again", sf::Color::White, {base_x + gap, base_y}, 24),
      client(clientManager)
{
    title.setFont(defaultFont);
    title.setString("Game End");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setPosition(270.f, 10.f);

    float baseX = 100.f;
    float baseY = 140.f;
    float spacing = 45.f;

    totalPoints.setFont(defaultFont);
    totalPoints.setCharacterSize(24);
    totalPoints.setFillColor(sf::Color::White);
    totalPoints.setPosition(baseX, baseY);

    gameTime.setFont(defaultFont);
    gameTime.setCharacterSize(24);
    gameTime.setFillColor(sf::Color::White);
    gameTime.setPosition(baseX, baseY + spacing);

    linesRemoved.setFont(defaultFont);
    linesRemoved.setCharacterSize(24);
    linesRemoved.setFillColor(sf::Color::White);
    linesRemoved.setPosition(baseX, baseY + spacing * 2);

    finalLevel.setFont(defaultFont);
    finalLevel.setCharacterSize(24);
    finalLevel.setFillColor(sf::Color::White);
    finalLevel.setPosition(baseX, baseY + spacing * 3);

    quitButton.setOnClick([this]()
                          {
                              screenManager.setActiveScreen("main-menu");
                              client->disconnect(); });

    playAgainButton.setOnClick([this]()
                               { client->request_game_start(); });
}

EndGameScreen::~EndGameScreen()
{
}

void EndGameScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    quitButton.handleEvent(event, manager.getWindow());
    playAgainButton.handleEvent(event, manager.getWindow());
}

void EndGameScreen::update(float deltaTime)
{

    nlohmann::json endGameData;

    if (client->hasEndGameData(endGameData))
    {
        data.deserialize(endGameData);

        totalPoints.setString("Total Points: " + std::to_string(data.totalPoints));
        gameTime.setString("Game Time: " + std::to_string(data.gameTime) + " seconds");
        linesRemoved.setString("Lines cleared: " + std::to_string(data.linesRemoved));
        finalLevel.setString("Final Level: " + std::to_string(data.finalLevel));

        float scoreboardX = 400.f;    // Right column X
        float scoreboardBaseY = 80.f; // Starting y for the scoreboard
        float scoreboardGap = 40.f;   // Vertical gap for each player line

        playerScores.clear();
        for (std::size_t i = 0; i < data.players.size(); ++i)
        {
            sf::Text scoreText;
            scoreText.setFont(defaultFont);
            scoreText.setCharacterSize(24);
            scoreText.setFillColor(sf::Color::Green);

            scoreText.setString("Player " + std::to_string(i + 1) + " - " + std::to_string(data.players[i].score) + " points");

            float currentY = scoreboardBaseY + i * scoreboardGap;
            scoreText.setPosition(scoreboardX, currentY);

            playerScores.push_back(scoreText);
        }
    }
}

void EndGameScreen::render(sf::RenderWindow &window)
{
    window.draw(title);

    window.draw(totalPoints);
    window.draw(gameTime);
    window.draw(linesRemoved);
    window.draw(finalLevel);

    for (auto &scoreText : playerScores)
        window.draw(scoreText);

    quitButton.render(window);
    playAgainButton.render(window);
}
