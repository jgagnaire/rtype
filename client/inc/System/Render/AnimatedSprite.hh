#ifndef ANIMATEDSPRITE_HH_VTLZOWBQ
#define ANIMATEDSPRITE_HH_VTLZOWBQ

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "System/Render/ADrawable.hh"

class AnimatedSprite : public sf::Drawable, public ADrawable
{
    public:
        AnimatedSprite();
        virtual ~AnimatedSprite();

        bool            load(const std::string &path,
                bool random = false, unsigned frameBySec = 10);
        virtual void    update(uint64_t);
        void            setPosition(const sf::Vector2f &pos);
        const sf::Vector2f &getPosition() const;
        virtual void    draw(sf::RenderTarget &target,
                sf::RenderStates states) const;
        uint64_t     getNbPlayed() const;
        void            setRepeat(bool);

    private:
        std::vector<sf::Sprite>         _sprites;
        sf::Texture                     *_texture;
        sf::Vector2f                    _position;
        int                             _current;
        int                             _width;
        int                             _height;
        bool                            _random;
        unsigned                        _frameBySec;
        uint64_t                     _save;
        uint64_t                     _numberPlayed;
        bool                            _repeat;
};

#endif /* end of include guard: ANIMATEDSPRITE_HH_VTLZOWBQ */
