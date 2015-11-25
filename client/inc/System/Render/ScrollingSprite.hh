#ifndef SCROLLINGSPRITE_HH_DAMIFJSE
# define SCROLLINGSPRITE_HH_DAMIFJSE

# include <SFML/Graphics.hpp>
# include "ADrawable.hh"

class ScrollingSprite : public sf::Drawable, public ADrawable
{
    public:
        ScrollingSprite();
        virtual ~ScrollingSprite();

        bool    load(const std::string &path,
                unsigned scrollingSpeed = 10,
                unsigned frameBySec = 10);
        virtual void    update();
        virtual void    draw(sf::RenderTarget &target,
                sf::RenderStates states) const;

    private:
        sf::Texture         _texture;
        sf::Sprite          _sprite;
        unsigned            _speed;
        unsigned            _frameBySec;
        unsigned            _left;
        sf::Clock           _clock;
};

#endif /* end of include guard: SCROLLINGSPRITE_HH_DAMIFJSE */
