#include "ShootSystem.hh"

ShootSystem::ShootSystem() : ASystem() {}

ShootSystem::~ShootSystem() {}

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
	
  void	line(Entity &e, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration + e.manager.get<float>("velocity");
    if (e.manager.get<bool>("is_a_monster"))
      vel *= -1;
    pos.first += vel;
    pos.second += sin(pos.first * 0.5 * M_PI / 90);
    e.manager.set("position", pos);
  }

  void	sinusoid(Entity &e, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration + e.manager.get<float>("velocity");
    if (e.manager.get<bool>("is_a_monster"))
      vel *= -1;
    pos.first += vel;
    pos.second += static_cast<float>(35 * sin(pos.first * 0.5 * M_PI / 180));
    e.manager.set("position", pos);
  }
}

void                    ShootSystem::update(int duration) {
  for (auto x = _entities.begin(); x != _entities.end();)
    {
      std::string name = (*x)->manager.get<std::string>("name");
      if (name == "normal_rotate")
	Pattern::sinusoid(**x, duration);
      if (name == "normal")
	Pattern::line(**x, duration);
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
