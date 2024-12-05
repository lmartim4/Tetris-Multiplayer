#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <map>

// Base class for screens
class Screen {
public:
    virtual ~Screen() {}
    virtual void handleEvent(sf::Event event, class ScreenManager& manager) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    void stop() {
        running = false;
    }

protected:
    std::atomic<bool> running{ true };
};

// Screen Manager
class ScreenManager {
    std::map<std::string, std::unique_ptr<Screen>> screens;
    Screen* activeScreen = nullptr;

    std::thread screenThread;
    std::mutex mtx;
    std::condition_variable cv;

    std::atomic<bool> threadRunning{ false };

public:
    ~ScreenManager() {
        stopThread();
    }

    void quit() {
        // Logic to terminate the application
        std::cout << "Application quitting...\n";
        exit(0);
    }

    void addScreen(const std::string& name, std::unique_ptr<Screen> screen) {
        screens[name] = std::move(screen);
    }

    void setActiveScreen(const std::string& name) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            activeScreen = screens[name].get();
        }
        cv.notify_one();
    }

    void handleEvent(sf::Event event) {
        if (activeScreen) {
            activeScreen->handleEvent(event, *this);
        }
    }

    void update(float deltaTime) {
        if (activeScreen) {
            activeScreen->update(deltaTime);
        }
    }

    void render(sf::RenderWindow& window) {
        if (activeScreen) {
            activeScreen->render(window);
        }
    }

    void startThread() {
        threadRunning = true;
        screenThread = std::thread([this]() {
            while (threadRunning) {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this]() { return activeScreen != nullptr; });
                lock.unlock();

                while (threadRunning) {
                    if (activeScreen) {
                        activeScreen->update(0.016f); // Simulate ~60 FPS updates
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(16));
                }
            } });
    }

    void stopThread() {
        threadRunning = false;
        if (screenThread.joinable()) {
            screenThread.join();
        }
    }
};