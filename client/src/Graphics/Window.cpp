#include "System/Render/Window.hh"
#include "System/Render/ADrawable.hh"
#include "System/Render/AView.hh"

Window::Window():
    _window(sf::VideoMode(1280, 720), "Pede"),
    _menuMode(true), _block(false)
{}

Window::~Window()
{}

void    Window::close()
{
    _window.close();
}

void    Window::clear()
{
    _window.clear();
}

void    Window::display()
{
    _window.display();
}

bool    Window::isOpen() const
{
    return _window.isOpen();
}

REvent  Window::getEvent()
{
    sf::Event e;

    if (_window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
            return (Key_Close);
        if (e.type == sf::Event::KeyReleased)
        {
            switch(e.key.code) {
                case sf::Keyboard::Tab :
                    return (Key_Change);
                case sf::Keyboard::Return :
                    return (Key_Select);
                case sf::Keyboard::BackSpace :
                    return (Key_Back);
                default:
                    return (noEvent);
            }
        }
    }
    if (_menuMode == false)
        _block = false;
    if (_menuMode && _block)
    {
        if (_clock.getElapsedTimeMilli() > 75)
        {
            _block = false;
            _clock.restart();
        }
        return (noEvent);
    }
    else if (_menuMode)
        _block = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        return (Key_Down);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        return (Key_Up);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        return (Key_Left);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        return (Key_Right);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        return (Key_Fire);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        return (Key_Charge);
    if (sf::Joystick::isConnected(0))
    {
        float x, y, z, r;
        x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
        y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
        z = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);
        r = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
        float &dir = ((x > 0 ? x : -x) > (y > 0 ? y : -y) ? x : y);
        if (sf::Joystick::isButtonPressed(0, 1))
            return (Key_Charge);
        if (z > -90.0)
            return (Key_Change);
        if (r > -90.0)
            return (Key_Fire);
        if (&dir == &x && x > 25)
            return (Key_Right);
        else if (&dir == &x && x < -25)
            return (Key_Left);
        else if (&dir == &y && y > 25)
            return (Key_Down);
        else if (&dir == &y && y < -25)
            return (Key_Up);
    }
    return (noEvent);
}

void  Window::draw(Entity &e)
{
    std::vector<ADrawable*> vec = e.manager.getAll<ADrawable*>();

    try {
        _window.setView(*(static_cast<const sf::View*>(e.manager.get<AView*>("view")->getBuffer())));
    } catch (const ComponentManagerException &)
    {}
    for (std::vector<ADrawable*>::iterator it = vec.begin();
            it != vec.end(); ++it)
    {
        const sf::Drawable *d = static_cast<const sf::Drawable *>((*it)->getBuffer());
        if (d)
            _window.draw(*d);
    }
}
