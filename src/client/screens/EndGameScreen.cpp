#include "EndGameScreen.hpp"
#include <string>
#include <iostream>

float base_x = 250;
float base_y = 380;
float gap = 180;

EndGameScreen::EndGameScreen(sf::RenderWindow &window, ScreenManager &screenManager, ClientManager &clientManager)
    : Screen(window),
      clientManager(clientManager), screenManager(screenManager),
      quitButton(defaultFont, "Quit", sf::Color::White, {base_x, base_y}, 24),
      playAgainButton(defaultFont, "Play Again", sf::Color::White, {base_x + gap, base_y}, 24)
{
    // ----------------------------------------------------------------------
    // Title (static)
    // ----------------------------------------------------------------------
    title.setFont(defaultFont);
    title.setString("Game End");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setPosition(270.f, 10.f);

    // ----------------------------------------------------------------------
    // Pre-set some default positions for labels. We'll fill their text later
    // once data is retrieved from the clientManager
    // ----------------------------------------------------------------------
    float baseX = 100.f;  // Left margin
    float baseY = 140.f;  // Starting y position for first stat
    float spacing = 45.f; // Vertical gap between lines

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

    // We do NOT fill them here with real data from `data`,
    // because we want to fetch it once in update().

    // ----------------------------------------------------------------------
    // Configure button callbacks
    // ----------------------------------------------------------------------
    quitButton.setOnClick([&]()
                          {
                              screenManager.setActiveScreen("main-menu");
                              clientManager.disconnect(); });

    playAgainButton.setOnClick([&]()
                               { clientManager.request_game_start(); });
}

EndGameScreen::~EndGameScreen()
{
}

void EndGameScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    // Let the buttons detect clicks
    quitButton.handleEvent(event, manager.getWindow());
    playAgainButton.handleEvent(event, manager.getWindow());

    // Possibly do something like manager.popScreen();
}

void EndGameScreen::update(float deltaTime)
{
    if (!hasFetchedData)
    {

        nlohmann::json endGameData;

        bool success = clientManager.hasEndGameData(endGameData);
        if (success)
        {
            data.deserialize(endGameData);

            // We got the data, now update our text fields:
            totalPoints.setString("Total Points: " + std::to_string(data.totalPoints));
            gameTime.setString("Game Time: " + std::to_string(data.gameTime) + " min");
            linesRemoved.setString("Lines removed: " + std::to_string(data.linesRemoved));
            finalLevel.setString("Final Level: " + std::to_string(data.finalLevel));

            // Also populate the "multiple player scores" column
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

                // Example: "j1 - PlayerName pts"
                scoreText.setString("j" + std::to_string(i + 1) +
                                    " - " + data.players[i].playerName + " pts");

                float currentY = scoreboardBaseY + i * scoreboardGap;
                scoreText.setPosition(scoreboardX, currentY);

                playerScores.push_back(scoreText);
            }

            // Set the flag so we never do this again
            hasFetchedData = true;
        }
    }
}

void EndGameScreen::render(sf::RenderWindow &window)
{
    // Draw title
    window.draw(title);

    // Draw the four stats
    window.draw(totalPoints);
    window.draw(gameTime);
    window.draw(linesRemoved);
    window.draw(finalLevel);

    // Draw each player's score
    for (auto &scoreText : playerScores)
    {
        window.draw(scoreText);
    }

    // Draw buttons
    quitButton.render(window);
    playAgainButton.render(window);
}
