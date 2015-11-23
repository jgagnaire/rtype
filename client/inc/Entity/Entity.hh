#ifndef ENTITY_H_
# define ENTITY_H_

#include "Entity/CompenentManager.hh"

class Entity
{
public:
  Entity() {}
  ~Entity() {}

  void			update() {}

  CompenentManager	manager;
};

#endif //ENTITY_H_
