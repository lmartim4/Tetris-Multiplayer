#include <iostream>
#include <enet/enet.h>

#include "CellRenderer.hpp"
#include "ClientManager.hpp"
#include "AudioManager.hpp"

#include "SoundType.hpp"

#include "screens/ScreenManager.hpp"
#include "screens/WaitingConnectionScreen.hpp"
#include "screens/GameScreen.hpp"
#include "screens/MenuScreen.hpp"
#include "screens/LobbyScreen.hpp"
#include "screens/BoardScreen.hpp"
#include "screens/EndGameScreen.hpp"

ScreenManager screenManager;
AudioManager audioManager;

ClientManager client(audioManager);

void heartbeat_listener(const Packet &packet)
{
    client.on_receive_heartbeat();
}

void onPlaySoundPacket(const Packet &packet)
{
    client.on_receive_play_sound(packet);
}

void onPlayerListPacket(const Packet &packet)
{
    client.on_receive_player_list(packet);
}

void onGameScreenPacket(const Packet &packet)
{
    client.on_receive_game_screen(packet);
}

void onGameEndPacket(const Packet &packet)
{
    screenManager.setActiveScreen("end-game");
    client.on_receive_end_screen(packet);
}

void onGameStartPacket(const Packet &packet)
{
    screenManager.setActiveScreen("game");
}

int main()
{
    audioManager.loadAllSounds();
    client.registerListener(PacketType::HEARTBEAT, heartbeat_listener);
    client.registerListener(PacketType::PLAYER_LIST, onPlayerListPacket);
    client.registerListener(PacketType::PLAY_SOUND, onPlaySoundPacket);
    client.registerListener(PacketType::GAME_SCREEN, onGameScreenPacket);
    client.registerListener(PacketType::STARTING_GAME, onGameStartPacket);
    client.registerListener(PacketType::ENG_GAME_SCREEN, onGameEndPacket);

    sf::RenderWindow window(sf::VideoMode(800, 480), "Multi-Threaded Screens");

    screenManager.addScreen("main-menu", std::make_unique<MenuScreen>(screenManager, client));
    screenManager.addScreen("lobby", std::make_unique<LobbyScreen>(client));
    screenManager.addScreen("waiting-connection", std::make_unique<WaitingConnectionScreen>(screenManager, client));
    screenManager.addScreen("game", std::make_unique<BoardScreen>(client));
    screenManager.addScreen("end-game", std::make_unique<EndGameScreen>(screenManager, client));

    screenManager.setActiveScreen("main-menu");
    screenManager.startThread();

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;

        float deltaTime = clock.restart().asSeconds();

        window.clear(sf::Color::Black);
        screenManager.render(window);
        window.display();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            screenManager.handleEvent(event);
        }
    }

    client.disconnect();
    return 0;
}