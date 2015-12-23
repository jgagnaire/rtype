#pragma once
#include <vector>
#include <list>
#include "ASystem.hh"
#include "Entity.hh"

class BossSystem : public ASystem
{
public:
  BossSystem();
  virtual ~BossSystem();

  virtual void                    update(int duration, GameRoom);
  virtual bool                    handle(const std::string &name,
					 Entity *e,
					 bool monster,
					 const Position &p);
private:
  int				   _fire_rate;
};
