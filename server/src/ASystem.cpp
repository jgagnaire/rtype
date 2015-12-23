#include "ASystem.hh"
#include "GameManager.hh"

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
typedef GameManager<SOCKET>				Manager;
# else
typedef GameManager<int>				Manager;
#endif

ASystem::ASystem() {}
ASystem::~ASystem() {}
std::string ASystem::_port;


void	ASystem::collision(System &system, Players &p, AllEntity &entities) {
  ASystem::checkFireCollision(system, p, entities);
  ASystem::collisionMonsterPlayer(system, p, entities);
  ASystem::collisionBonusPlayer(system, p, entities);
}

void	ASystem::line(Entity &e, int duration)
{
  std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
  float vel = static_cast<float>(duration * e.manager.get<float>("velocity"));
  if (e.manager.get<bool>("is_a_monster"))
    vel *= -1;
  pos.first += vel;
  pos.second += static_cast<float>(sin(pos.first * 0.5f * M_PI / 90.0f));
  e.manager.set("position", pos);
}

void	ASystem::sinusoid(Entity &e, int duration)
{
  std::pair<float, float> pos = e.manager.get<std::pair<float, float> >("position");
  float vel = static_cast<float>(duration * e.manager.get<float>("velocity"));
  if (e.manager.get<bool>("is_a_monster"))
    vel *= -1;
  pos.first += vel;
  pos.second += static_cast<float>(35 * sin(pos.first * 0.5 * M_PI / 180));
  e.manager.set("position", pos);
}

void	ASystem::sendCollision(uint64_t id1, uint64_t id2, Players &p) {
  Packet<UDPDataHeader> packet;
  UDPDataHeader		pack;

  std::string tmp = std::to_string(id1) + ":" + std::to_string(id2);
  std::cout << "je send l'id: " << tmp << std::endl;
  for (auto it = p.begin(); it != p.end(); ++it) {
    pack = {static_cast<uint16_t>(tmp.size()),
	    static_cast<uint16_t>(Enum::COLLISION),
	    (*it)->getUDPPacketId()};
      
    packet.stockOnBuff(pack);
    packet.stockOnBuff(tmp);
    packet.serialize();
    packet.sendPacket<ServerSocket *>(Manager::instance().getUDPSocket(),
				      (*it)->getIP(), ASystem::_port);
  }
}

void	ASystem::checkFireCollision(System &system, Players &p, AllEntity &entities) {
  for (auto shoot = system["shoot"]->_entities.begin();
       shoot != system["shoot"]->_entities.end();) {
    if (!(*shoot)->manager.get<bool>("is_a_monster")) {
      if (ASystem::touchMonster(*shoot, system, entities, p) ||
	  (ASystem::touchBoss(*shoot, system, entities, p)))
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

void   ASystem::collisionMonsterPlayer(System &system, Players &player, AllEntity &entities) {
  for (auto p = player.begin(); p != player.end(); ++p) {
    ASystem::touchPlayerMonster(system, *p, entities, player);
    ASystem::touchPlayerBoss(system, *p, entities, player);
  }
}

void   ASystem::collisionBonusPlayer(System &system, Players &player, AllEntity &entities) {
  for (auto p = player.begin(); p != player.end(); ++p)
    ASystem::touchPlayerBonus(system, *p, entities, player);
}

void	ASystem::touchPlayerBonus(System &system, User *player, AllEntity &entities, Players &p) {
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
      ASystem::sendCollision(player->getId(), (*m)->manager.get<uint64_t>("id"), p);
      player->getBonus(*m);
      delete *m;
      system["bonuses"]->_entities.erase(m++);
    }
    else
      ++m;
    if (player->isDead())
      return ;
  }
}

void	ASystem::touchPlayerBoss(System &system, User *player, AllEntity &entities, Players &p) {
  if (player->isRespawning())
    return ;
  Entity	&tmp_hitboxes = entities["hitboxes"];
  Entity	&hitboxes = tmp_hitboxes.manager.get<Entity>("hitboxes");
  Entity	&player_hitbox = hitboxes.manager.get<Entity>("player1");
  int		player_hitbox_x = player_hitbox.manager.get<int>("x");
  int		player_hitbox_y = player_hitbox.manager.get<int>("y");
  Position	player_pos = player->getPosition();

  for (auto m = system["boss"]->_entities.begin();
       m != system["boss"]->_entities.end();) {
    Entity	&monster_hitbox = hitboxes.manager.get<Entity>((*m)->manager.get<std::string>("name"));
    int       monster_hitbox_x = monster_hitbox.manager.get<int>("x");
    int       monster_hitbox_y = monster_hitbox.manager.get<int>("y");
    std::pair<float, float> monster_pos = (*m)->manager.get<std::pair<float, float> >("position");

    if (player_pos.x < monster_pos.first + monster_hitbox_x &&
	player_pos.x + player_hitbox_x > monster_pos.first &&
	player_pos.y < monster_pos.second + monster_hitbox_y &&
	player_hitbox_y + player_pos.y > monster_pos.second) {
      std::cout << "BOSS-PLAYER => TOUCHEY ! " << std::endl;
      ASystem::sendCollision(player->getId(), (*m)->manager.get<uint64_t>("id"), p);
      player->isTouched();
    }
    else
      ++m;
    return ;
  }
}

void	ASystem::touchPlayerMonster(System &system, User *player, AllEntity &entities, Players &p) {
  if (player->isRespawning())
    return ;
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
      ASystem::sendCollision(player->getId(), (*m)->manager.get<uint64_t>("id"), p);
      delete *m;
      player->isTouched();
      system["monsters"]->_entities.erase(m++);
      return ;
    }
    else
      ++m;
  }
}

bool	ASystem::touchPlayer(Entity *fire, Players &player, AllEntity &entities) {
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
    if ((*p)->isRespawning())
      continue ;
    Position player_pos = (*p)->getPosition();
      
    if (player_pos.x < fire_pos.first + fire_hitbox_x &&
	player_pos.x + player_hitbox_x > fire_pos.first &&
	player_pos.y < fire_pos.second + fire_hitbox_y &&
	player_hitbox_y + player_pos.y > fire_pos.second) {
      ASystem::sendCollision((*p)->getId(), fire->manager.get<uint64_t>("id"), player);
      std::cout << "FIRE-PLAYER => TOUCHEY !" << std::endl;
      (*p)->isTouched(fire->manager.get<int>("damage"));
      delete fire;
      return (true);
    }
  }
  return (false);
}

bool	ASystem::touchMonster(Entity *fire, System &system, AllEntity &entities, Players &p) {
  Entity	&tmp_hitboxes = entities["hitboxes"];
  Entity	&hitboxes = tmp_hitboxes.manager.get<Entity>("hitboxes");
  Entity	&fire_hitbox = hitboxes.manager.get<Entity>(fire->manager.get<std::string>("name"));
  int		fire_hitbox_x = fire_hitbox.manager.get<int>("x");
  int		fire_hitbox_y = fire_hitbox.manager.get<int>("y");
  std::pair<float, float> fire_pos = fire->manager.get<std::pair<float, float> >("position");

  bool has_been_del = false;
  for (auto m = system["monsters"]->_entities.begin();
       m != system["monsters"]->_entities.end();) {
    Entity	&monster_hitbox = hitboxes.manager.get<Entity>((*m)->manager.get<std::string>("name"));
    int       monster_hitbox_x = monster_hitbox.manager.get<int>("x");
    int       monster_hitbox_y = monster_hitbox.manager.get<int>("y");
    std::pair<float, float> monster_pos = (*m)->manager.get<std::pair<float, float> >("position");

	has_been_del = false;
    if (monster_pos.first < fire_pos.first + fire_hitbox_x &&
	monster_pos.first + monster_hitbox_x > fire_pos.first &&
	monster_pos.second < fire_pos.second + fire_hitbox_y &&
	monster_hitbox_y + monster_pos.second > fire_pos.second) {
      std::cout << "FIRE-MONSTER => TOUCHEY !" << std::endl;
      ASystem::sendCollision(fire->manager.get<uint64_t>("id"),
			     (*m)->manager.get<uint64_t>("id"), p);
      (*m)->manager.set<int>("life",
			     (*m)->manager.get<int>("life") - 
			     fire->manager.get<int>("damage"));
      if ((*m)->manager.get<int>("life") <=  0) {
		  delete *m;
		  m = system["monsters"]->_entities.erase(m);
		  has_been_del = true;
      }
      delete fire;
      return (true);
    }
	if (!has_been_del)
		++m;
  }    
  return (false);
}

bool	ASystem::touchBoss(Entity *fire, System &system, AllEntity &entities, Players &p) {
  Entity	&tmp_hitboxes = entities["hitboxes"];
  Entity	&hitboxes = tmp_hitboxes.manager.get<Entity>("hitboxes");
  Entity	&fire_hitbox = hitboxes.manager.get<Entity>(fire->manager.get<std::string>("name"));
  int		fire_hitbox_x = fire_hitbox.manager.get<int>("x");
  int		fire_hitbox_y = fire_hitbox.manager.get<int>("y");
  std::pair<float, float> fire_pos = fire->manager.get<std::pair<float, float> >("position");

  for (auto m = system["boss"]->_entities.begin();
       m != system["boss"]->_entities.end(); ++m) {
    Entity	&monster_hitbox = hitboxes.manager.get<Entity>((*m)->manager.get<std::string>("name"));
    int       monster_hitbox_x = monster_hitbox.manager.get<int>("x");
    int       monster_hitbox_y = monster_hitbox.manager.get<int>("y");
    std::pair<float, float> monster_pos = (*m)->manager.get<std::pair<float, float> >("position");

    if (monster_pos.first < fire_pos.first + fire_hitbox_x &&
	monster_pos.first + monster_hitbox_x > fire_pos.first &&
	monster_pos.second < fire_pos.second + fire_hitbox_y &&
	monster_hitbox_y + monster_pos.second > fire_pos.second) {
      std::cout << "FIRE-BOSS => TOUCHEY !" << std::endl;
      ASystem::sendCollision(fire->manager.get<uint64_t>("id"),
			     (*m)->manager.get<uint64_t>("id"), p);
      (*m)->manager.set<int>("life",
			     (*m)->manager.get<int>("life") - 
			     fire->manager.get<int>("damage"));
      if ((*m)->manager.get<int>("life") <=  0)
        (*m)->manager.add<bool>("is_dead", true);
      delete fire;
      return (true);
    }
  }    
  return (false);  
}
