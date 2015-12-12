#ifndef IWINDOW_HH_
# define IWINDOW_HH_

# include <string>
# include "System/Render/IEvent.hh"
# include "Entity/Entity.hh"
# include "System/Event/EventAggregator.hh"

class IWindow
{
    public:
        IWindow() {}
        virtual ~IWindow() {}

        virtual bool        isOpen() const = 0;
        virtual void        close() = 0;
        virtual void        clear() = 0;
        virtual void        display() = 0;
        virtual void        draw(Entity &e) = 0;
        virtual void        setMenuMode(bool) = 0;
        virtual REvents     getEvent() = 0;
};

#endif /* ifndef IWINDOW_HH_ */
