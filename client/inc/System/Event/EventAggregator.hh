#ifndef AGGREGATOR_HH_
# define AGGREGATOR_HH_

#include <unordered_map>
#include <vector>
#include <cstdint>

enum REvent
{
    noEvent = 0,
    Key_Up = 1,
    Key_Down = 2,
    Key_Left = 4,
    Key_Right = 8,
    Key_Fire = 16,
    Key_Charge = 32,
    Key_Change = 64,
    Key_Select,
    Key_Back,
    Key_Close,
    E_PlayOffline
};

typedef uint64_t REvents;

class ASystem;
class IWindow;

class EventAggregator
{
    private:
        IWindow						*win;
        std::unordered_map<ASystem*, std::vector<REvent> >	_systemList;
    public:
        EventAggregator(IWindow *w) : win(w) {}
        ~EventAggregator() {}
        void send(REvents);
        void add(REvent, ASystem*);
        void add(ASystem*);
        void update(void);
        inline IWindow *getWin() { return win; }
};

#endif // AGGREGATOR_HH_
