#include "BossSystem.hh"
#include "GameManager.hh"

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#  define _USE_MATH_DEFINES
#  include <math.h>
  typedef GameManager<SOCKET>			        gManager;
# else
#  include <cmath>
  typedef GameManager<int>			        gManager;
# endif

BossSystem::BossSystem() : ASystem(), _fire_rate(0) {}

BossSystem::~BossSystem() {}

void                    BossSystem::update(int duration, ASystem::GameRoom g) {
    for (auto x = _entities.begin(); x != _entities.end();)
    {
        std::string movement = (*x)->manager.get<std::string>("movement");
        if (movement == "sinusoid")
	  ASystem::sinusoid(**x, duration);
        if (movement == "line")
	  ASystem::line(**x, duration);
        if (movement == "boss2")
	  ASystem::boss2(**x, duration);
        if (movement == "boss1")
	  ASystem::boss1(**x, duration);
        if (movement == "mob1")
	  ASystem::mob1(**x, duration);
        if (movement == "mob2")
	  ASystem::mob2(**x, duration);
        std::pair<float, float> tmp = (*x)->manager.
            get<std::pair<float, float> >("position");
	_fire_rate += duration;
	if (_fire_rate >= (*x)->manager.get<int>("fire_rate")) {
	  Position p = {tmp.first, tmp.second};
	  _fire_rate = 0;
	  gManager::instance().fireMob(g, *x,
				       (*x)->manager.get<std::string>("name"), p);
	}
        if (tmp.first > 1920 || tmp.first < -100) {
	  (*x)->manager.add<bool>("is_dead", true);
	  return ;
	}
        else
            ++x;
    }
}

bool                    BossSystem::handle(const std::string &name,
					    Entity *e,
					    bool monster,
					    const Position &p) {
  e->manager.add("name", name);
  e->manager.add("is_a_monster", monster);
  e->manager.add("position", std::pair<float, float>(p.x, p.y));
  _entities.push_back(e);
  return (true);
}
