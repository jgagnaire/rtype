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
    float vel = duration + e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    pos.first += vel;
    pos.second += sin(pos.first * 0.5 * M_PI / 90);
    e.manager.set("position", pos);
  }

void	Pattern::sinusoid(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration + e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    pos.first += vel;
    pos.second += static_cast<float>(35 * sin(pos.first * 0.5 * M_PI / 180));
    e.manager.set("position", pos);
  }
