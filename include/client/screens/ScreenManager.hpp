#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

class Screen;

class ScreenManager
{
    sf::RenderWindow &window;
    std::map<std::string, std::unique_ptr<Screen>> screens;

    Screen *activeScreen = nullptr;
    std::thread screenThread;
    std::mutex mtx;
    std::condition_variable cv;

    std::atomic<bool> threadRunning{false};

public:
    ScreenManager(sf::RenderWindow &window);
    ~ScreenManager();

    sf::RenderWindow &getWindow() { return window; }

    void quit();
    void addScreen(const std::string &name, std::unique_ptr<Screen> screen);
    void setActiveScreen(const std::string &name);
    void handleEvent(sf::Event event);
    void update(float deltaTime);
    void render(sf::RenderWindow &window);
    void startThread();
    void stopThread();
};