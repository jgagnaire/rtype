#ifndef EVENT_HH_
# define EVENT_HH_

# include <Graphics/Window.hh>
# include "IEvent.hh"

class Event : public IEvent
{
    public:
        Event() {}
        virtual ~Event() {}

        virtual bool     isAccepted() const;
        virtual bool     isRefused() const;
        virtual bool     isCloseWindow() const;

        virtual bool     isFire() const;
        virtual bool     isRocket() const;
        virtual bool     isLeft() const;
        virtual bool     isRight() const;
        virtual bool     isUp() const;
        virtual bool     isDown() const;

        virtual void     *getBuffer();
    private:
        sf::Event       _event;
};

#endif /* ifndef EVENT_HH_ */
