#ifndef AGGREGATOR_HH_
# define AGGREGATOR_HH_

#include <unordered_map>
#include "ISystem.hh"

enum REvent
  {
    Key_Up,
    Key_Down,
    Key_Left,
    Key_Right,
    Key_Space,
    Key_Enter
  };

class EventAggregator
{
private:
  std::unordered_map<ISystem*, REvent*> _systemList;
public:
  EventAggregator() {}
  ~EventAggregator() {}
  void send(REvent);
  void add(REvent, ISystem&);
};

#endif // AGGREGATOR_HH_
