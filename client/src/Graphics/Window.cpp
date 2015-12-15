#include "System/Render/Window.hh"
#include "System/Render/ADrawable.hh"
#include "System/Render/AView.hh"

Window::Window():
    _window(sf::VideoMode(1920, 1080), "Pede"),
    _menuMode(true), _block(false)
{
    _window.setVerticalSyncEnabled(true);
}

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

EventSum Window::getEvent()
{
    sf::Event e;
    EventSum tmp = 0;

    if (_window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed
                || (e.type == sf::Event::KeyReleased
                    && e.key.code == sf::Keyboard::Escape))
            return (Key_Close);
        if (_menuMode && e.type == sf::Event::KeyReleased)
        {
            if (e.key.code == sf::Keyboard::BackSpace)
                return (127 | Key_Change);
            else if (e.key.code == sf::Keyboard::Return)
                return (126 | Key_Change);
            else if (e.key.code == sf::Keyboard::Tab)
                return (125 | Key_Change);
        }
        if (_menuMode && e.type == sf::Event::TextEntered)
        {
            if (e.text.unicode >= 'a' && e.text.unicode <= 'z')
                return (e.text.unicode | Key_Change);
            else
                return (Key_Change);
        }
        if (e.type == sf::Event::KeyReleased)
        {
            switch(e.key.code) {
			case sf::Keyboard::Tab :
				return (Key_Change);
			case sf::Keyboard::Return :
				return (Key_Select);
			case sf::Keyboard::BackSpace :
				return (Key_Back);
			case sf::Keyboard::K :
				return (Key_Sound);
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
        return (0);
    }
    else if (_menuMode)
        _block = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        tmp |= Key_Down;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        tmp |= Key_Up;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        tmp |= Key_Left;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        tmp |= Key_Right;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        tmp |= Key_Fire;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        tmp |= Key_Charge;
    if (tmp)
        return tmp;
    if (sf::Joystick::isConnected(0))
    {
        float x, y, z, r;
        x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
        y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
        z = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);
        r = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
        float &dir = ((x > 0 ? x : -x) > (y > 0 ? y : -y) ? x : y);
        if (z > -90.0)
            return ((_menuMode ? 125 | Key_Change : Key_Change));
        if (sf::Joystick::isButtonPressed(0, 1))
            tmp |= (_menuMode ? 127 | Key_Change : 0);
        if (sf::Joystick::isButtonPressed(0, 0))
            tmp |= (_menuMode ? 126 | Key_Change : Key_Charge);
        if (r > -90.0)
            tmp |= Key_Fire;
        if (&dir == &x && x > 25)
            tmp |= Key_Right;
        if (&dir == &x && x < -25)
            tmp |= Key_Left;
        if (&dir == &y && y > 25)
            tmp |= Key_Down;
        if (&dir == &y && y < -25)
            tmp |= Key_Up;
    }
    return (tmp);
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

void        Window::setMenuMode(bool m)
{
    _menuMode = m;
}
