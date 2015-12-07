#ifndef WINDOW_HH_
#define WINDOW_HH_

# include <SFML/Graphics.hpp>
# include "System/Render/IWindow.hh"
# include "Utility/Clock.hh"

class Window : public IWindow
{
    public:
        Window();
        virtual ~Window();

        virtual bool        isOpen() const;
        virtual void        close();
        virtual void        clear();
        virtual void        display();
        virtual void        draw(Entity &);
        virtual REvent      getEvent();

    private:
        sf::RenderWindow        _window;
        Clock                   _clock;
        bool                    _menuMode;
        bool                    _block;
};

#endif /* ifndef WINDOW_HH_ */
