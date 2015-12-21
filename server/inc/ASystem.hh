#pragma once
#include <vector>
#include <list>
#include "Entity.hh"
#include "UserManager.hh"

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
# else
#  include <cmath>
  typedef std::list<UserManager<int> *>			Players;
  typedef UserManager<int>				User;
#endif
  ASystem() {}
  virtual ~ASystem() {}

  virtual void	update(int duration) = 0;
  virtual bool	handle(const std::string &name, Entity *e,
		       bool monster, const Position &p) = 0;

  static void	collision(System &system, Players &p, AllEntity &entities) {
    ASystem::checkFireCollision(system, p, entities);
    ASystem::collisionMonsterPlayer(system, p, entities);
    ASystem::collisionBonusPlayer(system, p, entities);
  }

  static void	line(Entity &e, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = static_cast<float>(duration * e.manager.get<float>("velocity"));
    if (e.manager.get<bool>("is_a_monster"))
      vel *= -1;
    pos.first += vel;
    pos.second += static_cast<float>(sin(pos.first * 0.5f * M_PI / 90.0f));
    e.manager.set("position", pos);
  }

  static void	sinusoid(Entity &e, int duration)
  {
    std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
    float vel = static_cast<float>(duration * e.manager.get<float>("velocity"));
    if (e.manager.get<bool>("is_a_monster"))
      vel *= -1;
    pos.first += vel;
    pos.second += static_cast<float>(35 * sin(pos.first * 0.5 * M_PI / 180));
    e.manager.set("position", pos);
  }
protected:
  std::list<Entity *> _entities;

private:
  static void	checkFireCollision(System &system, Players &p, AllEntity &entities) {
    for (auto shoot = system["shoot"]->_entities.begin();
	 shoot != system["shoot"]->_entities.end();) {
      if (!(*shoot)->manager.get<bool>("is_a_monster")) {
	if (ASystem::touchMonster(*shoot, system, entities))
	  system["shoot"]->_entities.erase(shoot++);
	else
	  ++shoot;
      }
      else {
	if (ASystem::touchPlayer(*shoot, p, entities))
	  system["shoot"]->_entities.erase(shoot++);
	else
	  ++shoot;
      }
    }
  }

  static void   collisionMonsterPlayer(System &system, Players &player, AllEntity &entities) {
    for (auto p = player.begin(); p != player.end(); ++p)
      ASystem::touchPlayerMonster(system, *p, entities);
  }

  static void   collisionBonusPlayer(System &system, Players &player, AllEntity &entities) {
    for (auto p = player.begin(); p != player.end(); ++p)
      ASystem::touchPlayerBonus(system, *p, entities);
  }

  static void	touchPlayerBonus(System &system, User *player, AllEntity &entities) {
    Entity	&tmp_hitboxes = entities["hitboxes"];
    Entity	&hitboxes = tmp_hitboxes.manager.get<Entity>("hitboxes");
    Entity	&player_hitbox = hitboxes.manager.get<Entity>("player1");
    int		player_hitbox_x = player_hitbox.manager.get<int>("x");
    int		player_hitbox_y = player_hitbox.manager.get<int>("y");
    Position	player_pos = player->getPosition();

    for (auto m = system["bonuses"]->_entities.begin();
	 m != system["bonuses"]->_entities.end();) {
      Entity	&bonus_hitbox = hitboxes.manager.get<Entity>((*m)->manager.get<std::string>("name"));
      int       bonus_hitbox_x = bonus_hitbox.manager.get<int>("x");
      int       bonus_hitbox_y = bonus_hitbox.manager.get<int>("y");
      std::pair<float, float> bonus_pos = (*m)->manager.get<std::pair<float, float> >("position");

      if (player_pos.x < bonus_pos.first + bonus_hitbox_x &&
	  player_pos.x + player_hitbox_x > bonus_pos.first &&
	  player_pos.y < bonus_pos.second + bonus_hitbox_y &&
	  player_hitbox_y + player_pos.y > bonus_pos.second) {
	std::cout << "BONUS-PLAYER => TOUCHEY !" << std::endl;
	delete *m;
	system["bonuses"]->_entities.erase(m++);
      }
      else
	++m;
      if (player->isDead())
	return ;
    }
  }

  static void	touchPlayerMonster(System &system, User *player, AllEntity &entities) {
    Entity	&tmp_hitboxes = entities["hitboxes"];
    Entity	&hitboxes = tmp_hitboxes.manager.get<Entity>("hitboxes");
    Entity	&player_hitbox = hitboxes.manager.get<Entity>("player1");
    int		player_hitbox_x = player_hitbox.manager.get<int>("x");
    int		player_hitbox_y = player_hitbox.manager.get<int>("y");
    Position	player_pos = player->getPosition();

    for (auto m = system["monsters"]->_entities.begin();
	 m != system["monsters"]->_entities.end();) {
      Entity	&monster_hitbox = hitboxes.manager.get<Entity>((*m)->manager.get<std::string>("name"));
      int       monster_hitbox_x = monster_hitbox.manager.get<int>("x");
      int       monster_hitbox_y = monster_hitbox.manager.get<int>("y");
      std::pair<float, float> monster_pos = (*m)->manager.get<std::pair<float, float> >("position");

      if (player_pos.x < monster_pos.first + monster_hitbox_x &&
	  player_pos.x + player_hitbox_x > monster_pos.first &&
	  player_pos.y < monster_pos.second + monster_hitbox_y &&
	  player_hitbox_y + player_pos.y > monster_pos.second) {
	std::cout << "MONSTER-PLAYER => TOUCHEY !" << std::endl;
	delete *m;
	player->isTouched();
	system["monsters"]->_entities.erase(m++);
      }
      else
	++m;
      if (player->isDead())
	return ;
    }
  }

  static bool	touchPlayer(Entity *fire, Players &player, AllEntity &entities) {
    Entity	&tmp_hitboxes = entities["hitboxes"];
    Entity	&hitboxes = tmp_hitboxes.manager.get<Entity>("hitboxes");
    Entity	&fire_hitbox = hitboxes.manager.get<Entity>(fire->manager.get<std::string>("name"));
    Entity	&player_hitbox = hitboxes.manager.get<Entity>("player1");
    int		fire_hitbox_x = fire_hitbox.manager.get<int>("x");
    int		fire_hitbox_y = fire_hitbox.manager.get<int>("y");
    int		player_hitbox_x = player_hitbox.manager.get<int>("x");
    int		player_hitbox_y = player_hitbox.manager.get<int>("y");
    std::pair<float, float> fire_pos = fire->manager.get<std::pair<float, float> >("position");

    for (auto p = player.begin(); p != player.end(); ++p) {
      Position player_pos = (*p)->getPosition();
      
      if (player_pos.x < fire_pos.first + fire_hitbox_x &&
	  player_pos.x + player_hitbox_x > fire_pos.first &&
	  player_pos.y < fire_pos.second + fire_hitbox_y &&
	  player_hitbox_y + player_pos.y > fire_pos.second) {
	std::cout << "FIRE-PLAYER => TOUCHEY !" << std::endl;
	(*p)->isTouched();
	delete fire;
	return (true);
      }
    }
    return (false);
  }

  static bool	touchMonster(Entity *fire, System &system, AllEntity &entities) {
    Entity	&tmp_hitboxes = entities["hitboxes"];
    Entity	&hitboxes = tmp_hitboxes.manager.get<Entity>("hitboxes");
    Entity	&fire_hitbox = hitboxes.manager.get<Entity>(fire->manager.get<std::string>("name"));
    int		fire_hitbox_x = fire_hitbox.manager.get<int>("x");
    int		fire_hitbox_y = fire_hitbox.manager.get<int>("y");
    std::pair<float, float> fire_pos = fire->manager.get<std::pair<float, float> >("position");

    for (auto m = system["monsters"]->_entities.begin();
	 m != system["monsters"]->_entities.end(); ++m) {
      Entity	&monster_hitbox = hitboxes.manager.get<Entity>((*m)->manager.get<std::string>("name"));
      int       monster_hitbox_x = monster_hitbox.manager.get<int>("x");
      int       monster_hitbox_y = monster_hitbox.manager.get<int>("y");
      std::pair<float, float> monster_pos = (*m)->manager.get<std::pair<float, float> >("position");

      if (monster_pos.first < fire_pos.first + fire_hitbox_x &&
	  monster_pos.first + monster_hitbox_x > fire_pos.first &&
	  monster_pos.second < fire_pos.second + fire_hitbox_y &&
	  monster_hitbox_y + monster_pos.second > fire_pos.second) {
	std::cout << "MONSTER-FIRE => TOUCHEY !" << std::endl;
	delete *m;
	delete fire;
	system["monsters"]->_entities.erase(m);
	return (true);
      }
    }    
    return (false);
  }
};
