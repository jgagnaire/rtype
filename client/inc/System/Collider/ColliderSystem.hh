#ifndef COLLIDER_HH_
# define COLLIDER_HH_

#include <array>

class ColliderSystem : public ASystem
{
public:
	ColliderSystem() {}
	ColliderSystem(std::list<Entity*> *list) : isActiv(false), _eList(list) {}
	virtual ~ColliderSystem() {}
	
	virtual void                    update(int)
		{
			
		}
	virtual IPacket                 *out(EventSum &) { return NULL; }
	virtual void                    in(IPacket*) {}
	virtual bool                    handle(EventSum e) { return false; }
	virtual std::vector<REvent>     &broadcast(void) { return _eventList; }
	virtual EventSum                getEvent() { return noEvent; }
private:
	bool	isActiv;
	
};

#endif
