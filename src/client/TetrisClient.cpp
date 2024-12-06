#include <iostream>
#include "TetrisCell.hpp"

#include "screens/GameScreen.hpp"
#include "screens/MenuScreen.hpp"
#include "screens/LobbyScreen.hpp"
#include "screens/BoardScreen.hpp"
#include "screens/WaitingConnectionScreen.hpp"

#include <enet/enet.h>
#include "ClientManager.hpp"
#include "TetrisActions.hpp"

ClientManager client;

void heartbeat_listener(const Packet &packet)
{
    client.on_receive_heartbeat();
}

void onPlayerListPacket(const Packet &packet)
{
    nlohmann::json received;
    try
    {
        received = packet.toJson();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to parse packet as JSON: " << e.what() << std::endl;
        return;
    }

    if (!received.is_array())
    {
        std::cerr << "Invalid JSON: top-level element is not an array." << std::endl;
        return;
    }

    // Iterate over each inner array
    for (const auto &innerArray : received)
    {
        if (!innerArray.is_array())
        {
            std::cerr << "Inner element is not an array. Skipping." << std::endl;
            continue;
        }

        // Iterate over each player object in the inner arrays
        for (const auto &playerObject : innerArray)
        {
            if (!playerObject.is_object())
            {
                std::cerr << "Player entry is not a valid JSON object. Skipping." << std::endl;
                continue;
            }

            // Extract player fields with validation
            bool isConnected = false;
            int playerID = -1;
            std::string playerName = "Unknown";
            int score = 0;

            if (playerObject.contains("isConnected") && playerObject["isConnected"].is_boolean())
            {
                isConnected = playerObject["isConnected"].get<bool>();
            }
            if (playerObject.contains("playerID") && playerObject["playerID"].is_number_integer())
            {
                playerID = playerObject["playerID"].get<int>();
            }
            if (playerObject.contains("playerName") && playerObject["playerName"].is_string())
            {
                playerName = playerObject["playerName"].get<std::string>();
            }
            if (playerObject.contains("score") && playerObject["score"].is_number_integer())
            {
                score = playerObject["score"].get<int>();
            }

            // Now process the player data as needed.
            // For demonstration, we'll just print it.
            std::cout << "Player Info:\n";
            std::cout << "  IsConnected: " << (isConnected ? "true" : "false") << "\n";
            std::cout << "  PlayerID: " << playerID << "\n";
            std::cout << "  PlayerName: " << playerName << "\n";
            std::cout << "  Score: " << score << "\n\n";
        }
    }
}

int main()
{
    client.registerListener(PacketType::HEARTBEAT, heartbeat_listener);
    client.registerListener(PacketType::PLAYER_LIST, onPlayerListPacket);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Multi-Threaded Screens");
    ScreenManager screenManager;

    screenManager.addScreen("main-menu", std::make_unique<MenuScreen>(screenManager, client));
    screenManager.addScreen("lobby", std::make_unique<LobbyScreen>());
    screenManager.addScreen("waiting-connection", std::make_unique<WaitingConnectionScreen>(screenManager, client));
    screenManager.addScreen("game", std::make_unique<BoardScreen>());

    screenManager.setActiveScreen("main-menu");
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