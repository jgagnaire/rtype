#ifndef ENTITY_H_
# define ENTITY_H_

#include <functional>
#include "ComponentManager.hh"

class Entity
{
public:
  Entity() {}
  Entity(const Entity &e) : manager(e.manager) {}
  Entity&   operator=(const Entity &e)
  {
      if (this != &e)
      {
          Entity tmp(e);
          std::swap(tmp.manager, manager);
      }
      return (*this);
  }
  ~Entity() {}

  void			update() {}

  ComponentManager	manager;
};

#endif //ENTITY_H_
