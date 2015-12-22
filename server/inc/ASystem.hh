#pragma once
#include <vector>
#include <list>
#include "UserManager.hh"
#include "Enum.hh"
#include "Packet.hh"
#include "Entity.hh"

class ASystem
{
public:
  typedef std::unordered_map<std::string, ASystem *>	System;
  typedef std::unordered_map<std::string, Entity>       AllEntity;
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
#  define _USE_MATH_DEFINES
#  include <math.h>
  typedef std::list<UserManager<SOCKET> *>		Players;
  typedef UserManager<SOCKET>				User;
  typedef IServerSocket<SOCKET>				ServerSocket;
# else
#  include <cmath>
  typedef std::list<UserManager<int> *>			Players;
  typedef UserManager<int>				User;
  typedef IServerSocket<int>				ServerSocket;
#endif
  ASystem();
  virtual ~ASystem();

  virtual void	update(int duration) = 0;
  virtual bool	handle(const std::string &name, Entity *e,
		       bool monster, const Position &p) = 0;
  virtual std::list<Entity *> &getEntities() { return(_entities); }
  virtual void clear() {
    for (auto it = _entities.begin(); it != _entities.end(); ++it)
      delete *it;
    _entities.clear();
  }
  static void	collision(System &system, Players &p, AllEntity &entities);
  static void	line(Entity &e, int duration);
  static void	sinusoid(Entity &e, int duration);
protected:
  std::list<Entity *> _entities;

private:
  static void	sendCollision(std::size_t, std::size_t, Players &);
  static void	checkFireCollision(System &, Players &, AllEntity &);
  static void   collisionMonsterPlayer(System &, Players &, AllEntity &);
  static void   collisionBonusPlayer(System &, Players &, AllEntity &);
  static void	touchPlayerBonus(System &, User *, AllEntity &, Players &);
  static void	touchPlayerMonster(System &, User *, AllEntity &, Players &);
  static void	touchPlayerBoss(System &, User *, AllEntity &, Players &);
  static bool	touchPlayer(Entity *, Players &, AllEntity &);
  static bool	touchMonster(Entity *, System &, AllEntity &, Players &);
  static bool	touchBoss(Entity *, System &, AllEntity &, Players &);
};
