#pragma once
#include <vector>
#include <list>
#include "ASystem.hh"
#include "Entity.hh"

class ShootSystem : public ASystem
{
public:
  ShootSystem();
  virtual ~ShootSystem() {}

  virtual void                    update(int duration);
  virtual bool                    handle(const std::string &name,
					 Entity *e,
					 bool monster,
					 const Position &p);
private:
  std::list<Entity*>		_entities;
  int				fireRate;
};
