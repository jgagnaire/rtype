#include <iostream>
#include <cstdlib>
#include "AnimatedSprite.hh"

AnimatedSprite::AnimatedSprite():
    _current(0), _random(false)
{
    _buffer = this;
}

AnimatedSprite::~AnimatedSprite()
{}

bool AnimatedSprite::load(const std::string &path,
        bool random, unsigned frameBySec)
{
    unsigned width = 0;

    if (_texture.loadFromFile(path) == false)
        return (false);
    if (path.find("_") == std::string::npos)
        width = _texture.getSize().x;
    else
        width = std::atoi(&(path.c_str()[path.find("_") + 1]));
    _frameBySec = frameBySec;
    _random = random;
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
    if (_sprites.empty())
        return ;
    unsigned int mill = _clock.getElapsedTime().asMilliseconds();
    if (mill > 1000 / _frameBySec)
    {
        if (_random)
            _current = std::rand() % _sprites.size();
        else
            _current = (_current + mill / (1000 / _frameBySec)) % _sprites.size();
        _clock.restart();
    }
    _sprites[_current].setPosition(_position);
}

void    AnimatedSprite::draw(sf::RenderTarget &target,
        sf::RenderStates states) const
{
    if (_sprites.empty())
        return ;
    target.draw(_sprites[_current], states);
}

const sf::Vector2f &AnimatedSprite::getPosition() const
{
    return _position;
}
