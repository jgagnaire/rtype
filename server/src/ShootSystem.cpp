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

void                    ShootSystem::update(int duration) {
  for (auto x = _eList->begin(); x != _eList->end();)
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
}

bool                    ShootSystem::handle(const std::string &name,
					    Entity *e,
					    bool monster,
					    const Position &p) {
  e->manager.add("fired_by", name);
  e->manager.add("is_a_monster", monster);
  e->manager.add("position", std::pair<float, float>(p.x + 105.0f, p.y + 9.0f));
  _entities->push_back(e);
  return (true);
}
