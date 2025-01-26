#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Screen.hpp"
#include "Button.hpp"
#include "ClientManager.hpp"

class LobbyScreen : public Screen
{
    Button mainText;
    Button startGameText;
    std::shared_ptr<ClientManager> clientMan;
    const float circleRadius = 30.0f;
    const float padding = 100.0f;
    std::vector<Button> clickableTexts;

public:
    LobbyScreen(sf::RenderWindow &window, std::shared_ptr<ClientManager> clientManager);
    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;
};
