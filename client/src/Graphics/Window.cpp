#include "System/Render/Window.hh"
#include "System/Render/ADrawable.hh"
#include "System/Render/AView.hh"

Window::Window():
    _window(sf::VideoMode(1280, 720), "Pede")
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
        if (e.type == sf::Event::KeyPressed)
        {
            switch(e.key.code) {
                case sf::Keyboard::S :
                    return (Key_Fire);
                case sf::Keyboard::Space :
                    return (Key_Charge);
                case sf::Keyboard::Left :
                    return (Key_Left);
                case sf::Keyboard::Right :
                    return (Key_Right);
                case sf::Keyboard::Up :
                    return (Key_Up);
                case sf::Keyboard::Down :
                    return (Key_Down);
                default:
                    return (noEvent);
            }
        }
        else if (e.type == sf::Event::KeyReleased)
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
        else if (e.type == sf::Event::Closed)
            return (Key_Close);
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
