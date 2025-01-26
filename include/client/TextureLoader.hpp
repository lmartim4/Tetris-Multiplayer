#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>
#include <mutex>

class TextureLoader
{
public:
    static TextureLoader &getInstance()
    {
        static TextureLoader instance;
        return instance;
    }

    sf::Texture &getTexture(const std::string &name, const std::string &filePath)
    {
        std::lock_guard<std::mutex> lock(mtx);

        // Check if the texture is already loaded
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return *it->second;
        }

        // Load the texture and store it
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(filePath))
        {
            throw std::runtime_error("Failed to load texture: " + filePath);
        }

        auto &ref = *texture;
        textures[name] = std::move(texture);
        return ref;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mtx);
        textures.clear();
    }

private:
    TextureLoader() = default;
    ~TextureLoader() = default;

    TextureLoader(const TextureLoader &) = delete;
    TextureLoader &operator=(const TextureLoader &) = delete;

    std::map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::mutex mtx;
};
