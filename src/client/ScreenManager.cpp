#include "ScreenManager.hpp"
#include "Screen.hpp"

ScreenManager::ScreenManager(sf::RenderWindow &window) : window(window)
{
}

ScreenManager::~ScreenManager()
{
    stopThread();
}

void ScreenManager::quit()
{
    exit(0);
}

void ScreenManager::addScreen(const std::string &name, std::unique_ptr<Screen> screen)
{
    screens[name] = std::move(screen);
}

void ScreenManager::setActiveScreen(const std::string &name)
{
    {
        std::lock_guard<std::mutex> lock(mtx);

        sf::View defaultView(sf::FloatRect(
            0.f,
            0.f,
            static_cast<float>(window.getSize().x),
            static_cast<float>(window.getSize().y)));
        window.setView(defaultView);

        activeScreen = screens[name].get();
    }
    cv.notify_one();
}

void ScreenManager::handleEvent(sf::Event event)
{
    if (activeScreen)
    {
        activeScreen->handleEvent(event, *this);
    }
}

void ScreenManager::update(float deltaTime)
{
    if (activeScreen)
    {
        activeScreen->update(deltaTime);
    }
}

void ScreenManager::render(sf::RenderWindow &window)
{
    if (activeScreen)
        activeScreen->render(window);
}

void ScreenManager::startThread()
{
    threadRunning = true;
    screenThread = std::thread([this]()
                               {
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

void ScreenManager::stopThread()
{
    threadRunning = false;
    if (screenThread.joinable())
    {
        screenThread.join();
    }
}