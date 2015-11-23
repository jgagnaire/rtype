#include "Graphics/Event.hh"

void    *Event::getBuffer()
{
    return (&_event);
}

bool     Event::isAccepted() const
{
    return _event.type == sf::Event::KeyPressed &&
        _event.key.code == sf::Keyboard::Return;
}

bool     Event::isRefused() const
{
    return _event.type == sf::Event::KeyPressed &&
        _event.key.code == sf::Keyboard::Escape;
}

bool     Event::isCloseWindow() const
{
    return _event.type == sf::Event::Closed;
}

bool     Event::isFire() const
{
    return _event.type == sf::Event::KeyPressed &&
        _event.key.code == sf::Keyboard::Space;
}

bool     Event::isRocket() const
{
    return _event.type == sf::Event::KeyPressed &&
        _event.key.code == sf::Keyboard::V;
}

bool     Event::isLeft() const
{
    return _event.type == sf::Event::KeyPressed &&
        _event.key.code == sf::Keyboard::Left;
}

bool     Event::isRight() const
{
    return _event.type == sf::Event::KeyPressed &&
        _event.key.code == sf::Keyboard::Right;
}

bool     Event::isUp() const
{
    return _event.type == sf::Event::KeyPressed &&
        _event.key.code == sf::Keyboard::Up;
}

bool     Event::isDown() const
{
    return _event.type == sf::Event::KeyPressed &&
        _event.key.code == sf::Keyboard::Down;
}
