#include "System/Render/ScrollingSprite.hh"
#include "System/Render/TextureManager.hh"

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
    if ((_texture = TextureManager::getInstance().loadFromFile(path)) == 0)
        return false;
    _sprite.setTexture(*_texture);
    _sprite.setTextureRect(sf::IntRect(0, 0, 1920, 1080));
    return true;
}

void    ScrollingSprite::update(std::size_t duration)
{
    if (_texture == 0)
        return ;
    _save += duration;
    if (_save > 1000 / _frameBySec)
    {
        _left += (_save / (1000 / _frameBySec)) * _speed;
        if (_left + 1920 > _texture->getSize().x)
            _left -= _texture->getSize().x - 1920;
        _sprite.setTextureRect(sf::IntRect(_left,
                    0, 1920, 1080));
        _save = 0;
    }
}

void    ScrollingSprite::draw(sf::RenderTarget &target,
        sf::RenderStates states) const
{
    if (_texture == 0)
        return ;
    target.draw(_sprite, states);
}
