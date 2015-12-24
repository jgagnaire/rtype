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

void	Pattern::boss1(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration * e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    if (pos.first > 1200)
      line(e, s, duration);
    else if (pos.first <= 1200)
      {
	//	pos.second += static_cast<float>(sin(pos.second * 0.5f * M_PI / 90.0f));
	e.manager.set("position", pos);
      }
  }

void	Pattern::boss2(Entity &e, Side s, int duration)
{
  std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
  float vel = duration * e.manager.get<float>("velocity");
  if (s == Side::LEFT)
    vel *= -1;
  if (pos.first > 1200 && pos.second < 700)
    {
      pos.first += vel;
      pos.second += (vel / 0.2f) ;
    }
  else if (pos.first <= 1200 && pos.second < 700)
    {
      pos.first += vel;
      pos.second -= (vel);
    }
  else if (pos.second > 700)
    {
      if (pos.first > 1700)
	pos.second += vel;
      else
	pos.first -= vel;
    }
  e.manager.set("position", pos);
}

void	Pattern::mob1(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration * e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    if (pos.first > 1000)
      line(e, s, duration);
    else if (pos.first < 1000 && pos.first > 250 && pos.second < 800)
      {
	pos.second -= vel;
	e.manager.set("position", pos);
      }
    else if (pos.second >= 800 && pos.first > 200)
      {
    	line(e, s, duration);
	return ;
      }
    else if (pos.first <= 200 && pos.second > 200)
      {
    	pos.second += vel;
	e.manager.set("position", pos);
      }
    else if (pos.second <= 200 && pos.first <= 200)
      {
    	line(e, s, duration);
	return ;
      }
  }

void	Pattern::mob2(Entity &e, Side s, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = duration * e.manager.get<float>("velocity");
    if (s == Side::LEFT)
      vel *= -1;
    if (pos.first > 1000)
      line(e, s, duration);
    else if (pos.first < 1000 && pos.first > 250 && pos.second > 500)
      {
	pos.second += vel;
	e.manager.set("position", pos);
      }
    else if (pos.second <= 500 && pos.first > 200)
      {
    	line(e, s, duration);
	return ;
      }
    else if (pos.first <= 200 && pos.second > 200 && pos.second < 800)
      {
    	pos.second -= vel;
	e.manager.set("position", pos);
      }
    else if (pos.second >= 800 && pos.first <= 200)
      {
    	line(e, s, duration);
	return ;
      }
  }

std::function<void (Entity&, Pattern::Side, int)>   Pattern::getPattern(
        const std::string &name)
{
    if (name == "line")
        return &line;
    else if (name == "sinusoid")
        return &sinusoid;
    else if (name == "boss1")
        return &boss1;
    else if (name == "boss2")
      return &boss2;
    else if (name == "mob1")
      return &mob1;
    else if (name == "mob2")
      return &mob2;
    return &line;
}
