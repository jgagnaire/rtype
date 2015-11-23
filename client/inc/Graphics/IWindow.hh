#ifndef IWINDOW_HH_
# define IWINDOW_HH_

# include <string>
# include "Graphics/IEvent.hh"

class IWindow
{
    public:
        IWindow() {}
        virtual ~IWindow() {}

        virtual bool    isOpen() const = 0;
        virtual bool    getEvent(IEvent &) = 0;
};

#endif /* ifndef IWINDOW_HH_ */
