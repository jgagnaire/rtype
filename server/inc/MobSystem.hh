#pragma once
#include <vector>
#include <list>
#include "ASystem.hh"
#include "Entity.hh"

class MobSystem : public ASystem
{
public:
  MobSystem();
  virtual ~MobSystem();

  virtual void                    update(int duration);
  virtual bool                    handle(const std::string &name,
					 Entity *e,
					 bool monster,
					 const Position &p);
};
