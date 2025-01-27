#include <iostream>
#include <enet/enet.h>

#include "CellRenderer.hpp"
#include "ClientManager.hpp"

#include "AudioManager.hpp"
#include "ScreenManager.hpp"
#include "screens/WaitingConnectionScreen.hpp"
#include "screens/GameScreen.hpp"
#include "screens/MenuScreen.hpp"
#include "screens/LobbyScreen.hpp"
#include "screens/BoardScreen.hpp"
#include "screens/EndGameScreen.hpp"

sf::RenderWindow window(sf::VideoMode(900, 600), "Multi-Threaded Screens");

ScreenManager screenManager(window);
AudioManager audioManager;

std::shared_ptr<ClientManager> client;

void heartbeat_listener(const Packet &packet) { client->on_receive_heartbeat(); }
void onGameScore(const Packet &packet) { client->on_receive_score(packet); }
void onPlaySoundPacket(const Packet &packet)
{
    audioManager.playSound((SoundType)packet.getData()[0], false);
}

void onPlayerListPacket(const Packet &packet) { client->on_receive_player_list(packet); }

void onGameScreenPacket(const Packet &packet)
{
    screenManager.setActiveScreen("game");
    client->on_receive_game_screen(packet);
}

void onReceiveBackgroundSound(const Packet &packet)
{
    audioManager.playSound((SoundType)packet.getData()[0], true);
}

void onServerClosePacket(const Packet &packet)
{
    screenManager.setActiveScreen("main-menu");
    client->disconnect();
    std::cout << "Server closed";
}

void onGameEndPacket(const Packet &packet)
{
    screenManager.setActiveScreen("end-game");
    client->on_receive_end_screen(packet);
}

void onNextTetromino(const Packet &packet)
{
    client->on_receive_next_tetromino(packet);
}

void onGameStartPacket(const Packet &packet) { screenManager.setActiveScreen("game"); }
void onPlayerReceiveID(const Packet &packet) { client->on_receive_player_id(packet); }

int main()
{
    window.setFramerateLimit(60);
    audioManager.loadAllSounds();
    client = std::make_shared<ClientManager>();
    client->registerListener(PacketType::HEARTBEAT, heartbeat_listener);
    client->registerListener(PacketType::PLAYER_LIST, onPlayerListPacket);
    client->registerListener(PacketType::JOIN_ACCEPTED, onPlayerReceiveID);
    client->registerListener(PacketType::GAME_SCORE, onGameScore);
    client->registerListener(PacketType::PLAY_SOUND, onPlaySoundPacket);
    client->registerListener(PacketType::GAME_SCREEN, onGameScreenPacket);
    client->registerListener(PacketType::STARTING_GAME, onGameStartPacket);
    client->registerListener(PacketType::ENG_GAME_SCREEN, onGameEndPacket);
    client->registerListener(PacketType::NEXT_TETROMINO_DATA, onNextTetromino);
    client->registerListener(PacketType::SET_BACKGROUND_SOUND, onReceiveBackgroundSound);

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
    }

    client->disconnect();
    return 0;
}