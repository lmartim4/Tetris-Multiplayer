#include "EndGameScreen.hpp"
#include <string>
#include <iostream>

float base_x = 250;
float base_y = 380;
float gap = 180;
EndGameScreen::EndGameScreen(ClientManager &manager) : clientManager(manager),
                                                       quitButton(defaultFont, "Quit", sf::Color::White, {base_x, base_y}, 24),
                                                       playAgainButton(defaultFont, "Play Again", sf::Color::White, {base_x + gap, base_y}, 24)
{

    // ----------------------------------------------------------------------
    // 2) Title
    // ----------------------------------------------------------------------
    title.setFont(defaultFont);
    title.setString("Game End");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setPosition(270.f, 10.f);

    // ----------------------------------------------------------------------
    // Layout offsets for the left column of stats
    // ----------------------------------------------------------------------
    float baseX = 100.f;  // Left margin
    float baseY = 140.f;  // Starting y position for first stat
    float spacing = 45.f; // Vertical gap between lines

    // ----------------------------------------------------------------------
    // 3) Total points label
    // ----------------------------------------------------------------------
    totalPoints.setFont(defaultFont);
    totalPoints.setString("Total Points: " + std::to_string(data.totalPoints));
    totalPoints.setCharacterSize(24);
    totalPoints.setFillColor(sf::Color::White);
    totalPoints.setPosition(baseX, baseY);

    // ----------------------------------------------------------------------
    // 4) Game time label
    // ----------------------------------------------------------------------
    gameTime.setFont(defaultFont);
    gameTime.setString("Game Time: " + std::to_string(data.gameTime) + " min");
    gameTime.setCharacterSize(24);
    gameTime.setFillColor(sf::Color::White);
    gameTime.setPosition(baseX, baseY + spacing);

    // ----------------------------------------------------------------------
    // 5) Lines removed label
    // ----------------------------------------------------------------------
    linesRemoved.setFont(defaultFont);
    linesRemoved.setString("Lines removed: " + std::to_string(data.linesRemoved));
    linesRemoved.setCharacterSize(24);
    linesRemoved.setFillColor(sf::Color::White);
    linesRemoved.setPosition(baseX, baseY + spacing * 2);

    // ----------------------------------------------------------------------
    // 6) Final level label
    // ----------------------------------------------------------------------
    finalLevel.setFont(defaultFont);
    finalLevel.setString("Final Level: " + std::to_string(data.finalLevel));
    finalLevel.setCharacterSize(24);
    finalLevel.setFillColor(sf::Color::White);
    finalLevel.setPosition(baseX, baseY + spacing * 3);

    // ----------------------------------------------------------------------
    // 7) Multiple player scores column
    // ----------------------------------------------------------------------
    float scoreboardX = 400.f;    // Right column X
    float scoreboardBaseY = 80.f; // Starting y for the scoreboard
    float scoreboardGap = 40.f;   // Vertical gap for each player line

    for (std::size_t i = 0; i < data.players.size(); ++i)
    {
        sf::Text scoreText;
        scoreText.setFont(defaultFont);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Green);

        // Example: "j1 - PlayerName pts" (or adapt your string as needed)
        scoreText.setString("j" + std::to_string(i + 1) +
                            " - " + data.players[i].playerName + " pts");

        // Each row is offset by i * scoreboardGap
        float currentY = scoreboardBaseY + i * scoreboardGap;
        scoreText.setPosition(scoreboardX, currentY);

        playerScores.push_back(scoreText);
    }

    // ----------------------------------------------------------------------
    // 8) Configure button callbacks
    // ----------------------------------------------------------------------
    quitButton.setOnClick([&]()
                          {
                              std::cout << "[Quit button clicked]\n";
                              // You can signal handleEvent to pop the screen, for instance
                              // or call manager.popScreen() if manager is accessible here.
                          });

    playAgainButton.setOnClick([&]()
                               {
                                   std::cout << "[Play Again button clicked]\n";
                                   // Start a new game or replace the current screen, etc.
                               });
}

EndGameScreen::~EndGameScreen()
{
}

void EndGameScreen::handleEvent(sf::Event event, ScreenManager &manager)
{
    // Let the buttons detect clicks
    quitButton.handleEvent(event);
    playAgainButton.handleEvent(event);

    // You could also check here if the user actually clicked and then do
    // manager.popScreen(); or manager.pushScreen( ... );
}

void EndGameScreen::update(float /*deltaTime*/)
{
    // Anything you need updated continuously, e.g. animations
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
