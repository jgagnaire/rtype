#ifndef ISYSTEM_HH_
# define ISYSTEM_HH_

#include <chrono>
#include <vector>
#include "Event/EventAggregator.hh"
#include "Network/IPacket.hh"

class ISystem
{
public:
  ISystem() {}
  virtual ~ISystem() {}

  virtual void update(std::chrono::steady_clock) = 0;
  virtual IPacket *out(IPacket *) = 0;
  virtual bool handle(REvent e) = 0;
  virtual std::vector<REvent> broadcast(void) = 0;
protected:
  std::vector<REvent> _eventList;
};

#endif //ISYSTEM_HH_
