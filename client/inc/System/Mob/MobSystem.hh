#ifndef MOBSYSTEM_HH_
# define MOBSYSTEM_HH_

#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"

class	MobSystem : public ASystem
{
private:
	Entity *createMob(void);
public:
	MobSystem() {}
	MobSystem(std::list<Entity*> *list) : isActiv(false), _eList(list) {
		Entity *e = new Entity;

		e->manager.add<std::string>("name", "mob1");
		e->manager.add<std::string>("type", "mob");
		e->manager.add("position",
					   std::pair<float, float>(1000, 800));
		e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
			("pattern", Pattern::line);
		e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
		_eList->push_back(e);
	}
	virtual ~MobSystem() {}

	virtual void                    update(int duration)
		{
			if (!isActiv)
				return ;
			bool has_been_del = false;
			for (auto x = _eList->begin(); x != _eList->end();)
			{
				if ((*x)->manager.get<std::string>("type") == "mob")
				{
					(*x)->manager.get<std::function<void (Entity&, Pattern::Side, int)> >
                        ("pattern")(**x, (*x)->manager.
                                    get<Pattern::Side>("direction"), duration);
                    std::pair<float, float> tmp = (*x)->manager.
                        get<std::pair<float, float> >("position");
					if (tmp.first < 0)
					{
                        x = _eList->erase(x);
                        has_been_del = true;
                    }
				}
				if (!has_been_del)
                    ++x;
			}
		}
	virtual IPacket                 *out() { return NULL;}
	virtual void                    in(IPacket*) {}
		virtual bool                    handle(EventSum) { return true;}
	virtual std::vector<REvent>     &broadcast(void) { return _eventList; }
	virtual EventSum                getEvent() {return noEvent;}

protected:
	bool					isActiv;
	std::list<Entity*>	*_eList;
};

#endif
