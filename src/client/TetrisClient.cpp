#include <iostream>
#include <enet/enet.h>

#include "CellRenderer.hpp"
#include "ClientManager.hpp"

#include "AudioManager.hpp"
#include "screens/ScreenManager.hpp"
#include "screens/WaitingConnectionScreen.hpp"
#include "screens/GameScreen.hpp"
#include "screens/MenuScreen.hpp"
#include "screens/LobbyScreen.hpp"
#include "screens/BoardScreen.hpp"
#include "screens/EndGameScreen.hpp"

sf::RenderWindow window(sf::VideoMode(800, 480), "Multi-Threaded Screens");
ScreenManager screenManager(window);
AudioManager audioManager;

ClientManager client(audioManager);

void heartbeat_listener(const Packet &packet) { client.on_receive_heartbeat(); }
void onGameScore(const Packet &packet) { client.on_receive_score(packet); }
void onPlaySoundPacket(const Packet &packet) { client.on_receive_play_sound(packet); }

void onPlayerListPacket(const Packet &packet) { client.on_receive_player_list(packet); }

void onGameScreenPacket(const Packet &packet) { client.on_receive_game_screen(packet); }

void onGameEndPacket(const Packet &packet)
{
    screenManager.setActiveScreen("end-game");
    client.on_receive_end_screen(packet);
}

void onGameStartPacket(const Packet &packet) { screenManager.setActiveScreen("game"); }

int main()
{
    audioManager.loadAllSounds();

    client.registerListener(PacketType::HEARTBEAT, heartbeat_listener);
    client.registerListener(PacketType::PLAYER_LIST, onPlayerListPacket);
    client.registerListener(PacketType::GAME_SCORE, onGameScore);
    client.registerListener(PacketType::PLAY_SOUND, onPlaySoundPacket);
    client.registerListener(PacketType::GAME_SCREEN, onGameScreenPacket);
    client.registerListener(PacketType::STARTING_GAME, onGameStartPacket);
    client.registerListener(PacketType::ENG_GAME_SCREEN, onGameEndPacket);

    screenManager.addScreen("main-menu", std::make_unique<MenuScreen>(window, screenManager, client));
    screenManager.addScreen("lobby", std::make_unique<LobbyScreen>(window, client));
    screenManager.addScreen("waiting-connection", std::make_unique<WaitingConnectionScreen>(window, screenManager, client));
    screenManager.addScreen("game", std::make_unique<BoardScreen>(window, client));
    screenManager.addScreen("end-game", std::make_unique<EndGameScreen>(window, screenManager, client));

    screenManager.setActiveScreen("main-menu");
    screenManager.startThread();

    while (window.isOpen())
    {
        sf::Event event;

        window.clear(sf::Color::Black);
        screenManager.render(window);
        window.display();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            screenManager.handleEvent(event);
        }

        // std::cout << " x " << window.getSize().x << " || y = " << window.getSize().y << std::endl;
    }

    client.disconnect();
    return 0;
}