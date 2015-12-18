#include "System/Shoot/Pattern.hh"

Pattern::MovePattern Pattern::incremente(Pattern::MovePattern m)
  {
    switch (m)
      {
      case Pattern::MovePattern::LINE: return (Pattern::MovePattern::SINUS);
      case Pattern::MovePattern::SINUS: return (Pattern::MovePattern::LINE);
      default : return (Pattern::MovePattern::LINE);
      }
  }

void	Pattern::line(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration * e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    pos.first += vel;
	pos.second += static_cast<float>(sin(pos.first * 0.5f * M_PI / 90.0f));
    e.manager.set("position", pos);
  }

void	Pattern::sinusoid(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration * e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    pos.first += vel;
    pos.second += static_cast<float>(35 * sin(pos.first * 0.5f * M_PI / 180.0f));
    e.manager.set("position", pos);
  }

void	Pattern::mob1(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration * e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
	int idx = 1;
	if (pos.second >= 850)
		idx = -1;
	if (pos.second > 200 && pos.second < 850)
	{
		pos.second += (vel * -idx);
	} else
	{
		if (static_cast<int>(pos.first) % 10 == 0)
			pos.second -= (vel) * idx;
		else
			pos.first += vel;
	}
    e.manager.set("position", pos);
  }
