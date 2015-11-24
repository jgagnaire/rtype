#include "Graphics/Window.hh"
#include "Graphics/ADrawable.hh"
#include "Graphics/AView.hh"

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

bool  Window::getEvent(IEvent &event)
{
    sf::Event *e = static_cast<sf::Event*>(event.getBuffer());

    return _window.pollEvent(*e);
}

void  Window::draw(Entity &e)
{
    std::vector<ADrawable*> vec = e.manager.getAll<ADrawable*>();

    _window.setView(*(static_cast<const sf::View*>(e.manager.get<AView*>("view")->getBuffer())));
    for (std::vector<ADrawable*>::iterator it = vec.begin();
            it != vec.end(); ++it)
    {
        const sf::Drawable *d = static_cast<const sf::Drawable *>((*it)->getBuffer());
        if (d)
            _window.draw(*d);
    }
}
