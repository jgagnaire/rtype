#include "ShootSystem.hh"

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#  define _USE_MATH_DEFINES
#  include <math.h>
# else
#  include <cmath>
# endif

ShootSystem::ShootSystem() : ASystem() {}

ShootSystem::~ShootSystem() {}

void                    ShootSystem::update(int duration) {
    for (auto x = _entities.begin(); x != _entities.end();)
    {
        std::string name = (*x)->manager.get<std::string>("name");
        if (name == "rotate")
	  ASystem::sinusoid(**x, duration);
        if (name == "normal")
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

bool                    ShootSystem::handle(const std::string &name,
					    Entity *e,
					    bool monster,
					    const Position &p) {
  e->manager.add("fired_by", name);
  e->manager.add("is_a_monster", monster);
  e->manager.add("position", std::pair<float, float>(p.x + 105.0f, p.y + 9.0f));
  _entities.push_back(e);
  return (true);
}
