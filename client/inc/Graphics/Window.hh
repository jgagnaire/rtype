#ifndef WINDOW_HH_
#define WINDOW_HH_

# include <SFML/Window.hpp>
# include "Graphics/IWindow.hh"

class Window : public IWindow
{
    public:
        Window();
        virtual ~Window();

        virtual bool    isOpen() const;
        virtual void    close();
        virtual bool    getEvent(IEvent &);

    private:
        sf::Window              _window;
};

#endif /* ifndef WINDOW_HH_ */
