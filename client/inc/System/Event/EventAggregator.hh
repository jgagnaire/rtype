#ifndef AGGREGATOR_HH_
# define AGGREGATOR_HH_

#include <unordered_map>
#include <vector>

enum REvent
{
    noEvent = -1,
    Key_Up,
    Key_Down,
    Key_Left,
    Key_Right,
    Key_Fire,
    Key_Charge,
    Key_Change,
    Key_Select,
    Key_Back,
    Key_Close
};

class ISystem;
class IWindow;

class EventAggregator
{
    private:
        IWindow						*win;
        std::unordered_map<ISystem*, std::vector<REvent> >	_systemList;
    public:
        EventAggregator(IWindow *w) : win(w) {}
        ~EventAggregator() {}
        void send(REvent);
        void add(REvent, ISystem*);
        void add(ISystem*);
        void update(void);
        inline IWindow *getWin() { return win; }
};

#endif // AGGREGATOR_HH_
