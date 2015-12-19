#ifndef TEXTUREMANAGER_HH_UP0SF8OC
#define TEXTUREMANAGER_HH_UP0SF8OC

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <iostream>

class TextureManager
{
    private:
    TextureManager()
    {
        _textures.reserve(40);
    }
    ~TextureManager() {
        for (auto x : _textures)
        {
            delete x.second;
        }
    }

    public:
    static TextureManager   &getInstance()
    {
        static TextureManager      tm;
        return tm;
    }

    sf::Texture      *loadFromFile(const std::string &file)
    {
        sf::Texture         *t;

        if (_textures[file])
        {
            return _textures[file];
        }
        else
        {
            t = new sf::Texture;
            if (t->loadFromFile(file))
            {
                _textures[file] = t;
                return t;
            }
            else
                delete t;
        }
        return 0;
    }

    private:
        std::unordered_map<std::string, sf::Texture*>        _textures;
};

#endif /* end of include guard: TEXTUREMANAGER_HH_UP0SF8OC */
