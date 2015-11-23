#ifndef IWINDOW_HH_
# define IWINDOW_HH_

# include <string>
# include "Graphics/IEvent.hh"
# include "Graphics/IDrawable.hh"

class IWindow
{
    public:
        IWindow() {}
        virtual ~IWindow() {}

        virtual bool    isOpen() const = 0;
        virtual void    close() = 0;
        virtual void    clear() = 0;
        virtual void    display() = 0;
        virtual void    draw(IDrawable &) = 0;
        virtual bool    getEvent(IEvent &) = 0;
};

#endif /* ifndef IWINDOW_HH_ */
