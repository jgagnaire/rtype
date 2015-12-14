
#ifndef SHOOTSYSTEM_HH_
# define SHOOTSYSTEM_HH_

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#  define _USE_MATH_DEFINES
#  include <ATLComTime.h>
# else
#  include <cmath>
# endif

# include <functional>
# include "System/ASystem.hh"

namespace Pattern {
	
  enum MovePattern {LINE = 0, SINUS = 1};
	
  enum class Side {LEFT, RIGHT};

  Pattern::MovePattern incremente(Pattern::MovePattern m)
  {
    switch (m)
      {
      case Pattern::MovePattern::LINE: return (Pattern::MovePattern::SINUS);
      case Pattern::MovePattern::SINUS: return (Pattern::MovePattern::LINE);
      default : return (Pattern::MovePattern::LINE);
      }
  }
	
  void	line(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration + e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    pos.first += vel;
    pos.second += sin(pos.first * 0.5 * M_PI / 90);
    e.manager.set("position", pos);
  }

  void	sinusoid(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration + e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    pos.first += vel;
    pos.second += static_cast<float>(35 * sin(pos.first * 0.5 * M_PI / 180));
    e.manager.set("position", pos);
  }
}

class ShootSystem : public ASystem
{
public:
  ShootSystem(std::list<Entity*> *_list) : _eList(_list), fireRate(250), isActiv(false)
  {
    _eventList.push_back(Key_Fire);
    _eventList.push_back(Key_Charge);
    _eventList.push_back(Key_Change);
    _eventList.push_back(E_Stage);
    patterns[0] = Pattern::line;
    patterns[1] = Pattern::sinusoid;
  }
  virtual ~ShootSystem() {}
	
  virtual void                    update(int duration)
  {
    if (!isActiv)
      return ;
    if ((this->fireRate -= duration) <= 0)
      this->fireRate = 250;		
    for (auto x = _eList->begin(); x != _eList->end();)
      {
	bool has_been_del = false;
	if ((*x)->manager.get<std::string>("type") == "shoot")
	  {
	    (*x)->manager.get<std::function<void (Entity&, Pattern::Side, int)> >
	      ("pattern")(**x, (*x)->manager.
			  get<Pattern::Side>("direction"), duration);
	    std::pair<float, float> tmp = (*x)->manager.
	      get<std::pair<float, float> >("position");
	    if (tmp.first > 1920 || tmp.first < 0)
	      {
		x = _eList->erase(x);
		has_been_del = true;
	      }
	  }
	if (!has_been_del)
	  ++x;
      }
  }
	
  virtual IPacket                 *out() {/*envoi de packet quand on tire*/ return NULL; }
  virtual void                    in(IPacket*) {}
  virtual bool                    handle(EventSum ev)
  {
    if (ev == E_Stage)
      isActiv = !isActiv;
    if (ev & Key_Fire && this->fireRate == 250 && isActiv)
      {
	Entity *e = new Entity;

	e->manager.add<std::string>("name", "playerShoot");
	e->manager.add<std::string>("type", "shoot");
	e->manager.add("velocity", 4.50f);
	e->manager.add<size_t>("dammage", 25);
	for(auto x : *_eList)
	  if (x->manager.get<std::string>("name") == "player1")
	    {
	      e->manager.add("position",
			     std::pair<float, float>(
						     x->manager.get<std::pair<float, float> >
						     ("position").first + 105.0f,
						     x->manager.get<std::pair<float, float> >
						     ("position").second + 9.0f));
	      e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
		("pattern",
		 patterns[x->manager.get<Pattern::MovePattern>("pattern")]);
	    }
	e->manager.add<Pattern::Side>("direction", Pattern::Side::RIGHT);
	_eList->push_back(e);
      }
    else if (ev & Key_Change && isActiv)
      {
	for(auto x : *_eList)
	  if (x->manager.get<std::string>("name") == "player1")
	    {
	      x->manager.set<Pattern::MovePattern>
		("pattern", Pattern::incremente
		 (x->manager.get<Pattern::MovePattern>("pattern")));
	      break ;
	    }
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
  int					fireRate;
  bool				isActiv;
  std::function<void (Entity&, Pattern::Side, int)> patterns[2];
};

#endif //SHOOTSYSTEM_HH_
