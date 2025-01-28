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

    // Load a texture from file or retrieve it if already loaded
    sf::Texture &getTexture(const std::string &name, const std::string &filePath)
    {
        auto it = textures.find(name);
        if (it == textures.end())
        {
            sf::Texture tex;
            if (!tex.loadFromFile(filePath))
            {
                std::cerr << "Failed to load texture: " << filePath << std::endl;
                // Handle error appropriately, possibly throw an exception
            }
            textures[name] = std::move(tex);
        }
        return textures[name];
    }

    // Extract a tile from an atlas and return it as a separate texture
    sf::Texture &getTile(const std::string &atlasName, const std::string &tileName, const sf::IntRect &tileRect)
    {
        std::string key = atlasName + "_" + tileName;
        auto it = textures.find(key);
        if (it == textures.end())
        {
            sf::Texture tileTexture;
            sf::Texture &atlas = getTexture(atlasName, atlasFilePaths[atlasName]);

            // Create a new texture for the tile
            if (!tileTexture.loadFromImage(getTileImage(atlas, tileRect)))
            {
                std::cerr << "Failed to extract tile: " << tileName << " from atlas: " << atlasName << std::endl;
                // Handle error appropriately
            }

            tileTexture.setRepeated(true); // Enable texture repeating
            textures[key] = std::move(tileTexture);
        }
        return textures[key];
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

    sf::Image getTileImage(const sf::Texture &atlas, const sf::IntRect &tileRect)
    {
        sf::Image atlasImage = atlas.copyToImage();
        sf::Image tileImage;
        tileImage.create(tileRect.width, tileRect.height, sf::Color::Transparent);

        for (int y = 0; y < tileRect.height; ++y)
        {
            for (int x = 0; x < tileRect.width; ++x)
            {
                tileImage.setPixel(x, y, atlasImage.getPixel(tileRect.left + x, tileRect.top + y));
            }
        }

        return tileImage;
    }

    std::map<std::string, std::string> atlasFilePaths;
    std::map<std::string, sf::Texture> textures;
    std::mutex mtx;
};
