#ifndef IEVENT_HH_
# define IEVENT_HH_

class IEvent
{
    public:
        IEvent() {}
        virtual ~IEvent() {}

        virtual bool     isAccepted() const = 0;
        virtual bool     isRefused() const = 0;
        virtual bool     isCloseWindow() const = 0;

        virtual bool     isFire() const = 0;
        virtual bool     isRocket() const = 0;
        virtual bool     isLeft() const = 0;
        virtual bool     isRight() const = 0;
        virtual bool     isUp() const = 0;
        virtual bool     isDown() const = 0;

        virtual void     *getBuffer() = 0;
};

#endif /* ifndef IEVENT_HH_ */
