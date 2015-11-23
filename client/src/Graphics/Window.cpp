#include "Graphics/Window.hh"

Window::Window():
    _window(sf::VideoMode::getDesktopMode(), "Hair tip")
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

bool  Window::getEvent(IEvent &event)
{
    sf::Event *e = static_cast<sf::Event*>(event.getBuffer());

    return _window.pollEvent(*e);
}
