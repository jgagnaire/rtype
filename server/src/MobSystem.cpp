#include "MobSystem.hh"

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#  define _USE_MATH_DEFINES
#  include <math.h>
# else
#  include <cmath>
# endif

MobSystem::MobSystem() : ASystem() {}

MobSystem::~MobSystem() {}

void                    MobSystem::update(int duration) {
    for (auto x = _entities.begin(); x != _entities.end();)
    {
        std::string movement = (*x)->manager.get<std::string>("movement");
        if (movement == "sinusoid")
	  ASystem::sinusoid(**x, duration);
        if (movement == "line")
	  ASystem::line(**x, duration);
        std::pair<float, float> tmp = (*x)->manager.
            get<std::pair<float, float> >("position");
        if (tmp.first > 1920 || tmp.first < 0) {
            delete *x;
            _entities.erase(x++);
        }
        else
            ++x;
    }
}

bool                    MobSystem::handle(const std::string &name,
					  Entity *e,
					  bool monster,
					  const Position &p) {
  e->manager.add("name", name);
  e->manager.add("position", std::pair<float, float>(p.x, p.y));
  e->manager.add("is_a_monster", monster);
  _entities.push_back(e);
  return (true);
}
