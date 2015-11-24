#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Graphics/ADrawable.hh"

class AnimatedSprite : public sf::Drawable, public ADrawable
{
    public:
        AnimatedSprite();
        virtual ~AnimatedSprite();

        bool            load(const std::string &path, int width);
        virtual void    update();
        void            setPosition(const sf::Vector2f &pos);
        virtual void    draw(sf::RenderTarget &target,
                sf::RenderStates states) const;

    private:
        std::vector<sf::Sprite>         _sprites;
        sf::Texture                     _texture;
        sf::Clock                       _clock;
        sf::Vector2f                    _position;
        int                             _current;
        int                             _width;
};
