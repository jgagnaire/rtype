#include <iostream>
#include "AnimatedSprite.hh"

AnimatedSprite::AnimatedSprite():
    _current(0)
{
    _buffer = this;
}

AnimatedSprite::~AnimatedSprite()
{}

bool AnimatedSprite::load(const std::string &path, int width)
{
    if (_texture.loadFromFile(path) == false)
        return (false);
    _texture.setSmooth(true);
    int nbFrame = _texture.getSize().x / width;
    int height = _texture.getSize().y;
    _sprites.reserve(nbFrame);
    for (int i = 0; i < nbFrame; ++i)
    {
        _sprites.push_back(sf::Sprite(_texture,
                    sf::IntRect(i * width, 0, width, height)));
    }
    _position.x = 0;
    _position.y = 0;
    return (true);
}

void        AnimatedSprite::setPosition(const sf::Vector2f &pos)
{
    _position = pos;
}

void    AnimatedSprite::update()
{
    int mill = _clock.getElapsedTime().asMilliseconds();
    if (mill > 30)
    {
        _current = (_current + mill / 30) % _sprites.size();
        _clock.restart();
    }
    _sprites[_current].setPosition(_position);
}

void    AnimatedSprite::draw(sf::RenderTarget &target,
        sf::RenderStates states) const
{
    target.draw(_sprites[_current], states);
}
