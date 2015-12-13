#ifndef ASYSTEM_HH_
# define ASYSTEM_HH_

#include <vector>
#include <list>
#include <Entity/Entity.hh>
#include "Event/EventAggregator.hh"
#include "Network/IPacket.hh"
#include "Utility/IClock.hh"

class ASystem
{
public:
	ASystem() {}
	ASystem(std::list<Entity*> *);
	virtual ~ASystem() {}

	virtual void                    update(int duration) = 0;
	virtual IPacket                 *out() = 0;
	virtual void                    in(IPacket*) = 0;
	virtual bool                    handle(EventSum e) = 0;
	virtual std::vector<REvent>     &broadcast(void) = 0;
	virtual EventSum                getEvent() = 0;

    virtual void draw() {}
protected:
	std::vector<REvent> _eventList;
};

#endif //ASYSTEM_HH_
