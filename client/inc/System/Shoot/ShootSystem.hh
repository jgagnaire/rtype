#ifndef SHOOTSYSTEM_HH_
# define SHOOTSYSTEM_HH_

#include <functional>
#include <System/ASystem.hh>

namespace Pattern {
	
	enum class Side {LEFT, RIGHT};
	
	void	line(Entity &e, Side s, int duration)
	{
		std::pair<float, float> pos = e.manager.get("position");
		float vel = e.manager.get("velocity");
		if (s == LEFT)
			vel *= -1;
		pos.second += vel;
		e.manager.set("position", pos);
	}
}

class ShootSystem : public ASystem
{
public:
	ShootSystem(std::list<Entity*> *_list) : _eList(_list) {}
	virtual ~ShootSystem() {}
	
	virtual void                    update(int duration)
		{
			for(auto x : _eList)
			{
				if (x->manager.get<std::string>("type") == "shoot")
				{
					/*nique ta mere*/
				}
			}
		}
	
	virtual IPacket                 *out() {/*envoi de packet quand on tire*/}
	virtual void                    in(IPacket*) { return NULL }
	virtual bool                    handle(EventSum e)
		{
			if (e == Key_Fire)
			{
				Entity *e = new Entity;
				
				e->manager.add("name", "playerShoot");
				e->manager.add("type", "shoot");
				e->manager.add("velocity", 1.35f);
				for(auto x : *_eList)
					if (x->manager.get<std::string>("name") == "player1")
					{
						e->manager.add("position",
									   x->manager.
									   get<std::pair<float, float> >("position"));
					}
				e->manager.add<Pattern::Side>("direction", Pattern::RIGHT);
				e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
					("pattern", Pattern::line);
				_eList.push_back(e);
			}
		}
	virtual std::vector<REvent>     &broadcast(void)
		{
			return (_eventList);
		}
	
	virtual EventSum                getEvent()
		{
			return (noEvent);
		}
protected:
	std::list<Entity*>	*_eList;
};

#endif //SHOOTSYSTEM_HH_
