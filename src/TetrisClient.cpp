#include <iostream>
#include "Screen.hpp"

#include "screens/GameScreen.hpp"
#include "screens/MenuScreen.hpp"

#include <enet/enet.h>
#include "ClientManager.hpp"
#include "TetrisActions.hpp"

ClientManager client;

void heartbeat_listener(const Packet &packet)
{
    client.on_receive_heartbeat();
}

int main()
{
    client.registerListener(PacketType::HEARTBEAT, heartbeat_listener);

    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Multi-Threaded Screens");
    ScreenManager screenManager;
    
    screenManager.addScreen("menu", std::make_unique<MenuScreen>(screenManager));
    screenManager.addScreen("game", std::make_unique<GameScreen>());


    screenManager.setActiveScreen("menu");
    screenManager.startThread();

    sf::Clock clock;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            screenManager.handleEvent(event);
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear(sf::Color::Black);
        screenManager.render(window);
        window.display();
    }
    
    while (client.isRunning())
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return 0;
}