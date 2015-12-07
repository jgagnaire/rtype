#include "System/Render/ScrollingSprite.hh"

ScrollingSprite::ScrollingSprite():
    _left(0), _save(0)
{
    _buffer = this;
}

ScrollingSprite::~ScrollingSprite()
{}

bool    ScrollingSprite::load(const std::string &path,
        unsigned scrollingSpeed, unsigned frameBySec)
{
    _speed = scrollingSpeed;
    _frameBySec = frameBySec;
    if (_texture.loadFromFile(path) == false)
        return false;
    _sprite.setTexture(_texture);
    _sprite.setTextureRect(sf::IntRect(0, 0, 1920, 1080));
    return true;
}

void    ScrollingSprite::update(std::size_t duration)
{
    _save += duration;
    if (_save > 1000 / _frameBySec)
    {
        _left += (_save / (1000 / _frameBySec)) * _speed;
        if (_left + 1920 > _texture.getSize().x)
            _left -= _texture.getSize().x - 1920;
        _sprite.setTextureRect(sf::IntRect(_left,
                    0, 1920, 1080));
        _save = 0;
    }
}

void    ScrollingSprite::draw(sf::RenderTarget &target,
        sf::RenderStates states) const
{
    target.draw(_sprite, states);
}
