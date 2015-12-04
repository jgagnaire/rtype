#ifndef AGGREGATOR_HH_
# define AGGREGATOR_HH_

#include <unordered_map>
#include <System/Render/IWindow.hh>
#include <System/Render/Window.hh>
#include "System/ISystem.hh"

enum REvent
  {
    Key_Up,
    Key_Down,
    Key_Left,
    Key_Right,
    Key_Space,
    Key_Enter
  };

class ISystem;

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
  void update(void);
  inline IWindow *getWin() { return win; }
};

#endif // AGGREGATOR_HH_
