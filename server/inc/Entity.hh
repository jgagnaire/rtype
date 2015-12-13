#ifndef ENTITY_H_
# define ENTITY_H_

#include "ComponentManager.hh"

class Entity
{
public:
  Entity() {}
  ~Entity() {}

  void			update() {}

  ComponentManager	manager;
};

#endif //ENTITY_H_
