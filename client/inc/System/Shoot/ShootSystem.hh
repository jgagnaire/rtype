#ifndef SHOOTSYSTEM_HH_
# define SHOOTSYSTEM_HH_

#include <functional>
#include <System/ASystem.hh>

namespace Pattern {
	
	enum class Side {LEFT, RIGHT};
	
	void	line(Entity &e, Side s, int duration)
	{
		std::cout << "LINE" << std::endl;
		std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
		std::cout << "Before : " << pos.first << " : " << pos.second << std::endl;
		float vel = duration + e.manager.get<float>("velocity");
		if (s == Side::LEFT)
			vel *= -1;
		pos.first += vel;
		e.manager.set("position", pos);
		std::cout << "After : " << e.manager.get<std::pair<float, float> >("position").first << " : " << e.manager.get<std::pair<float, float> >("position").second << std::endl;
	}
}

class ShootSystem : public ASystem
{
public:
	ShootSystem(std::list<Entity*> *_list) : _eList(_list)
		{
			_eventList.push_back(Key_Fire);
		}
	virtual ~ShootSystem() {}
	
	virtual void                    update(int duration)
		{
			for(auto x = _eList->begin(); x != _eList->end(); ++x)
			{
				if ((*x)->manager.get<std::string>("type") == "shoot")
				{
					(*x)->manager.get<std::function<void (Entity&, Pattern::Side, int)> >
						("pattern")(**x, (*x)->manager.
									get<Pattern::Side>("direction"), duration);
					std::pair<float, float> tmp = (*x)->manager.
						get<std::pair<float, float> >("position");
					if (tmp.first > 1920 || tmp.second > 1080
						|| tmp.first < 0 || tmp.second < 0)
					{
						std::cout << _eList->size() << std::endl;
						_eList->erase(x);
						//delete x;
						std::cout << _eList->size() << std::endl;
						std::cout << "LES FESSES CA DELETE" << std::endl;
					}
				}
			}
			std::cout << "Bye bye" << std::endl;
		}
	
	virtual IPacket                 *out() {/*envoi de packet quand on tire*/ return NULL; }
	virtual void                    in(IPacket*) {}
	virtual bool                    handle(EventSum e)
		{
			if (e == Key_Fire)
			{
				Entity *e = new Entity;
				
				e->manager.add<std::string>("name", "playerShoot");
				e->manager.add<std::string>("type", "shoot");
				e->manager.add("velocity", 1.35f);
				e->manager.add<size_t>("dammage", 25);  
				for(auto x : *_eList)
					if (x->manager.get<std::string>("name") == "player1")
					{
						e->manager.add("position",
									   x->manager.
									   get<std::pair<float, float> >("position"));
					}
				e->manager.add<Pattern::Side>("direction", Pattern::Side::RIGHT);
				e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
					("pattern", Pattern::line);
				_eList->push_back(e);
			}
			return true;
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
