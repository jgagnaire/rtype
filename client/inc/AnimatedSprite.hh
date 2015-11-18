#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class AnimatedSprite
{
    public:
        AnimatedSprite();
        virtual ~AnimatedSprite() {}

        bool        load(const std::string &path, int width);
        void        setPosition(const sf::Vector2f &pos);
        sf::Sprite  &draw();

    private:
        std::vector<sf::Sprite>         _sprites;
        sf::Texture                     _texture;
        sf::Clock                       _clock;
        sf::Vector2f                    _position;
        int                             _current;
};
