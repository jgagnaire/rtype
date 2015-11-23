#include "Graphics/Window.hh"
#include "Graphics/ADrawable.hh"

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

void  Window::draw(Entity &e)
{
    std::vector<ADrawable*> vec = e.manager.getAll<ADrawable*>();

    for (std::vector<ADrawable*>::iterator it = vec.begin();
            it != vec.end(); ++it)
    {
        const sf::Drawable *d = static_cast<const sf::Drawable *>((*it)->getBuffer());
        _window.draw(*d);
    }
}
