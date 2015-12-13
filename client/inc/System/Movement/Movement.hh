#ifndef MOVEMENT_HH_
# define MOVEMENT_HH_

#include "System/ASystem.hh"

class MovementSystem : public ASystem
{
public:
	MovementSystem() : lastEvent(noEvent) {};
	MovementSystem(std::list<Entity*> *eList) : _eList(eList), lastEvent(noEvent) {
		_eventList.push_back(Key_Up);
		_eventList.push_back(Key_Down);
		_eventList.push_back(Key_Left);
		_eventList.push_back(Key_Right);
	}
	
	virtual ~MovementSystem() {};
	
	virtual void                    update(int duration)
		{
			float culpointastah = 1.0f;
		    
			for (unsigned i = 1; i < sizeof(EventSum) * 8; i <<= 1)
			{
				if (lastEvent & i)
					++culpointastah;
				if (culpointastah >= 2)
					break ;
			}
			
			float move  = duration + (0.75f / culpointastah);
			for(auto x : *_eList)
			{
				if (x->manager.get<std::string>("name") == "player1")
				{
					std::pair<float, float> tmp =
						x->manager.get<std::pair<float, float> >("position");
					if (lastEvent & Key_Up)
						tmp.second -= move;
					if (lastEvent & Key_Down)
						tmp.second += move;
					if (lastEvent & Key_Left)
						tmp.first -= move;
					if (lastEvent & Key_Right)
						tmp.first += move;
					x->manager.set("position", tmp);
					if (lastEvent != 0)
						lastEvent = 0;
					break;
				}
			}
		}
	virtual IPacket                 *out() { return NULL; }
	virtual void                    in(IPacket*){}
	virtual bool                    handle(EventSum e)
		{
            if (e & Key_Up || e & Key_Down || e & Key_Left || e & Key_Right)
				lastEvent = e;
			return (true);
		}
	virtual std::vector<REvent>     &broadcast(void)
		{
			return (_eventList);
		}
	
	virtual EventSum                getEvent()
		{
			return (lastEvent);
		}
	
protected:
	std::list<Entity*>	*_eList;
	EventSum			lastEvent;
};

#endif // MOVEMENT_HH_
