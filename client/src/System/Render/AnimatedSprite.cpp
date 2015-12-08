#include <iostream>
#include <cstdlib>
#include "System/Render/AnimatedSprite.hh"

AnimatedSprite::AnimatedSprite():
    _current(0), _random(false), _save(0)
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
    _width = width;
    _height = _texture.getSize().y;
    return (true);
}

void        AnimatedSprite::setPosition(const sf::Vector2f &pos)
{
    _position = pos;
    if (_position.x < 0)
        _position.x = 0;
    if (_position.y < 0)
        _position.y = 0;
    if (_position.x + _width > 1920)
        _position.x = 1920 - _width;
    if (_position.y + _height > 1080)
        _position.y = 1080 - _height;
}

void    AnimatedSprite::update(std::size_t duration)
{
    if (_sprites.empty())
        return ;
    if (_sprites.size() > 0)
    {
        _save += duration;
        if (_save > 1000 / _frameBySec)
        {
            if (_random)
                _current = std::rand() % _sprites.size();
            else
                _current = (_current + _save / (1000 / _frameBySec)) % _sprites.size();
            _save = 0;
        }
        _sprites[_current].setPosition(_position);
    }
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
