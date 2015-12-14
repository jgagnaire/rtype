#pragma once
#include <vector>
#include <list>
#include "Entity.hh"
#include "UserManager.hh"

class ASystem
{
public:
  ASystem() {}
  virtual ~ASystem() {}

  virtual void                    update(int duration) = 0;
  virtual bool                    handle(const std::string &name,
					 Entity *e,
					 bool monster,
					 const Position &p) = 0;
	// virtual std::vector<REvent>     &broadcast(void) = 0;
	// virtual EventSum                getEvent() = 0;
};
