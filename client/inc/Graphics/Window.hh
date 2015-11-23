#ifndef WINDOW_HH_
#define WINDOW_HH_

# include <SFML/Graphics.hpp>
# include "Graphics/IWindow.hh"

class Window : public IWindow
{
    public:
        Window();
        virtual ~Window();

        virtual bool    isOpen() const;
        virtual void    close();
        virtual void    clear();
        virtual void    display();
        virtual void    draw(Entity &);
        virtual bool    getEvent(IEvent &);

    private:
        sf::RenderWindow       _window;
};

#endif /* ifndef WINDOW_HH_ */
