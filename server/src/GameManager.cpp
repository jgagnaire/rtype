#include <chrono>
#include "GameManager.hh"

template <typename SCK>
GameManager<SCK> *GameManager<SCK>::game_manager = 0;

template <typename SCK>
Entity GameManager<SCK>::configuration;

template <typename SCK>
GameManager<SCK>::GameManager() {}

template <typename SCK>
GameManager<SCK>     &GameManager<SCK>::instance() {
  if (GameManager::game_manager == 0) {
    JSONParser	*p;

    JSONParser::parseFile("./entities/configuration.json");
    p = JSONParser::parse();
    GameManager<SCK>::configuration = p->getEntity().manager.get<Entity>("configuration");
    GameManager<SCK>::game_manager = new GameManager();
    delete p;
  }
  return (*game_manager);
}

template <typename SCK>
Game<SCK>        *GameManager<SCK>::getGameByName(const std::string &name) {
    for (auto it = _games.begin(); it != _games.end(); ++it) {
        if ((*it)->name == name)
            return (*it);
    }
    return 0;
}

template <typename SCK>
void        GameManager<SCK>::createRoom(const std::string &name, UserManager<SCK> *s) {
    Game<SCK>    *g = new Game<SCK>;
    const std::string strs[] = {"fires", "levels", "monsters",
				"bonuses", "hitboxes"};
    JSONParser	*jp;

    g->name = name;
    g->players.push_back(s);
    for (std::size_t i = 0; i < sizeof(strs) / sizeof(strs[0]); ++i) {
      JSONParser::parseFile(GameManager<SCK>::configuration.manager.get<std::string>(strs[i]));
      jp = JSONParser::parse();
      g->entities[strs[i]] = jp->getEntity();
      g->content_system[strs[i]] = JSONParser::getContent();
      delete jp;
    }
    _games.push_back(g);
}

template <typename SCK>
void        GameManager<SCK>::deleteUser(UserManager<SCK> *u) {
    Game<SCK> *g = getGameByName(u->getGameroomName());

    if (!g)
        return ;
    for (auto it = g->players.begin(); it != g->players.end(); ++it) {
        if (u->getName() == (*it)->getName()) {
	    std::cout << u->getName() << " est trouve dans la room " << g->name << std::endl;
            g->players.erase(it);
            break ;
        }
    }
    if (g->players.empty()) {
        for (auto it = _games.begin(); it != _games.end(); ++it) {
            if (g->name == (*it)->name) {
                _games.erase(it);
                break ;
            }
        }
    }
}

template <typename SCK>
void        GameManager<SCK>::fireBall(Game<SCK> *game, UserManager<SCK> *u,
				       bool second_weapon) {
  Entity	*ent;
  Entity	&tmp = game->entities["fires"];

  if (second_weapon)
    ent = new Entity(tmp.manager.get<Entity>("fires").manager.get<Entity>("rotate"));
  else
    ent = new Entity(tmp.manager.get<Entity>("fires").manager.get<Entity>("normal"));
  ent->manager.add<std::size_t>("id", game->shoot_player_ids++);
  game->system["shoot"]->handle(u->getName(), ent, false, u->getPosition());
}

template <typename SCK>
bool        GameManager<SCK>::joinRoom(const std::string &name, UserManager<SCK> *u) {
    if (roomIsFull(name))
        return (false);
    Game<SCK> *game = getGameByName(name);

    if (!game)
        return false;
    game->players.push_back(u);
    return true;
}

template <typename SCK>
bool        GameManager<SCK>::isAllDead(Game<SCK> *game) const {
  for (auto p = game->players.begin(); p != game->players.end(); ++p){
    if (!(*p)->isDead())
      return (false);
  }
  return (true);
}

template <typename SCK>
bool        GameManager<SCK>::roomIsFull(const std::string &name) {
    Game<SCK> *game = getGameByName(name);

    if (!game)
        return false;
    return (game->players.size() == 4);
}

template <typename SCK>
void        GameManager<SCK>::launchGame(const std::string &game_name) {
    Game<SCK>    *game = getGameByName(game_name);

    if (game && !game->is_playing) {
        game->is_playing = true;
        _threadpool.startThread(_threadpool.add(&GameManager::createGame, game));
    }
}

template <typename SCK>
void        GameManager<SCK>::updatePositions(Game<SCK> *game, std::size_t time) {
    for (auto it = game->players.begin(); it != game->players.end(); ++it)
        (*it)->changePosition(time);
}

template <typename SCK>
bool        GameManager<SCK>::updateTime(Game<SCK> *game) {
    Entity	&tmp = game->entities["levels"];
    Entity	&entity =
      tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);
    std::size_t	game_time = entity.manager.get<int>("time");
    std::size_t	tmp_time = getTimeInSecond() + entity.manager.get<int>("time_tmp") - game_time;

    entity.manager.set<int>("time_tmp", entity.manager.get<int>("time_tmp") - tmp_time);
    entity.manager.set<int>("timeleft", entity.manager.get<int>("timeleft") + tmp_time);
    return (entity.manager.get<int>("time_tmp") <= 0);
}

template <typename SCK>
bool
GameManager<SCK>::checkEntities(Game<SCK> *game,
				std::pair<std::string, Entity&> entity,
				int time,
				std::size_t duration,
				const std::string &ent_name) {
  Entity	&tmp = game->entities[ent_name];
  Entity	&entity_pos = entity.second.manager.get<Entity>("position");
  Position	pos = {entity_pos.manager.get<float>("x"),
		       entity_pos.manager.get<float>("y")};

  if (time >= entity.second.manager.get<int>("appear_at_sec")) {
    if (entity.second.manager.exist<int>("refresh")) {
      if (entity.second.manager.get<int>("timeval") <=
	  entity.second.manager.get<int>("refresh")) {
	Entity	*ent =
	  new Entity(tmp.manager.get<Entity>(ent_name).manager.get<Entity>(entity.first));
	if (ent_name == "bonuses")
	  ent->manager.add<std::size_t>("id", game->bonus_ids++);
	else
	  ent->manager.add<std::size_t>("id", game->monster_ids++);
	entity.second.manager.set<int>("refresh", time);
	entity.second.manager.set<int>("time", entity.second.manager.get<int>("time") - 1);
	game->system[ent_name]->handle(entity.first, ent, true, pos);
      }
      else {
	entity.second.manager.set<int>("refresh",
					entity.second.manager.get<int>("refresh")
					+ duration);
      }
    }
    else {
      Entity	*ent =
	new Entity(tmp.manager.get<Entity>(ent_name).manager.get<Entity>(entity.first));
      entity.second.manager.add<int>("refresh", duration);
      if (ent_name == "bonuses")
	ent->manager.add<std::size_t>("id", game->bonus_ids++);
      else
	ent->manager.add<std::size_t>("id", game->monster_ids++);
      game->system[ent_name]->handle(entity.first, ent, true, pos);
      if (entity.second.manager.exist<int>("time"))
	entity.second.manager.set<int>("time",
				       entity.second.manager.get<int>("time") - 1);
      else
	return (true);
    }
  }
  if (entity.second.manager.exist<int>("time"))
    return (entity.second.manager.get<int>("time") <= 0);
  return (false);
}

template <typename SCK>
bool        GameManager<SCK>::updateObjSighting(Game<SCK> *game, std::size_t time,
						     const std::string &ent_name) {
  Entity	&tmp = game->entities["levels"];
  Entity	&entity =
    tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);
  auto		&objs = entity.manager.get<std::vector<Entity> >(ent_name);

  for (auto obj = objs.begin(); obj != objs.end();) {
    if (objs.size() == 0)
      return (true);
    auto m = obj->manager.getAll<Entity>();
    if (m.empty())
      return (true);
    std::pair<std::string, Entity&> tmp_obj = {m[0].first,
						   obj->manager.get<Entity>(m[0].first)}; 
    if (!m.empty() && checkEntities(game, tmp_obj,
				    entity.manager.get<int>("timeleft"), time, ent_name))
      objs.erase(obj++);
    else
      ++obj;
  }
  return (true);
}

template <typename SCK>
bool        GameManager<SCK>::updateBoss(Game<SCK> *game) {
  Entity	&tmp = game->entities["levels"];
  Entity	&tmp_monster = game->entities["monsters"];
  Entity	&monster = tmp_monster.manager.get<Entity>("monsters");
  Entity	&entity =
    tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);
  Entity	&boss = entity.manager.get<Entity>("boss");

  if (boss.manager.exist<bool>("in_game"))
    return (true);

  const std::string	&boss_name= boss.manager.get<std::string>("name");
  Entity	boss_pos = boss.manager.get<Entity>("position");
  Position	p = {boss_pos.manager.get<float>("x"), boss_pos.manager.get<float>("y")};

  boss.manager.add<bool>("in_game", true);
  Entity *ent =
    new Entity(monster.manager.get<Entity>(boss_name));
  ent->manager.add<std::size_t>("id", game->boss_ids++);
  std::cout << "ET LE BOSS: " << boss_name << " EST LA !!" << std::endl;
  game->system["boss"]->handle(boss_name, ent, true, p);
  return (true);
}

template <typename SCK>
bool        GameManager<SCK>::bossIsDead(Game<SCK> *game) {
  std::list<Entity *> &ents = game->system["boss"]->getEntities();

  if (ents.empty())
    return (false);
  return ((*ents.begin())->manager.exist<bool>("is_dead"));
}

template <typename SCK>
bool        GameManager<SCK>::update(Game<SCK> *game, std::size_t time) {
  if (updateTime(game))
    updateBoss(game);
    updatePositions(game, time);
    updateObjSighting(game, time, "monsters");
    updateObjSighting(game, time, "bonuses");
    game->system["shoot"]->update(time);
    game->system["monsters"]->update(time);
    game->system["bonuses"]->update(time);
    game->system["boss"]->update(time);
    for (auto p = game->players.begin(); p != game->players.end(); ++p)
      if (!(*p)->isDead())
	(*p)->updateBonus(time);
    ASystem::collision(game->system, game->players, game->entities);
    return (!(game->players.empty() ||
	      isAllDead(game) || bossIsDead(game)));
}

template <typename SCK>
std::size_t   GameManager<SCK>::getTimeInSecond() {
    return (static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch() /
           std::chrono::seconds(1)));
}

template <typename SCK>
std::size_t   GameManager<SCK>::getTime() {
    return (static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1)));
}

template <typename SCK>
void            GameManager<SCK>::synchronisation(Game<SCK> *game) {
  const std::size_t   *latency = (*game->players.begin())->getLatency();
  std::size_t	duration = ((latency[1] - latency[0]) + (latency[3] - latency[2])) / 2;

  duration = duration + GameManager<SCK>::getTime();
  (*game->players.begin())->writeStruct({0, Enum::GAME_START});
  while (duration > GameManager<SCK>::getTime());
}

template <typename SCK>
bool            GameManager<SCK>::gameTransition(Game<SCK> *game) {
  char		lvl = game->lvl_name.back();

  if (lvl >= '5')
    return (false);
  game->lvl_name.pop_back();
  game->lvl_name.push_back(lvl + 1);
  game->system["shoot"]->clear();
  game->system["monsters"]->clear();
  game->system["bonuses"]->clear();
  game->system["boss"]->clear();
  for (auto p = game->players.begin(); p != game->players.end(); ++p)
    (*p)->clearLevel();
  return (true);
}

template <typename SCK>
void            GameManager<SCK>::createGame(Game<SCK> *game) {
    GameManager<SCK>	&g = GameManager<SCK>::instance();
    //	g.synchronisation(game);
    bool		is_not_finished = true;
    std::size_t		duration;
    auto		start = std::chrono::steady_clock::now();
    Entity		&tmp = game->entities["levels"];
    Entity		&tmp_entity =
      tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);

    std::cout << "C'est partie pour le niveau " << game->lvl_name << std::endl;
    tmp_entity.manager.add<int>("time_tmp", tmp_entity.manager.get<int>("time"));
    tmp_entity.manager.add<int>("timeleft", 0);
    tmp_entity.manager.set<int>("time", getTimeInSecond() + tmp_entity.manager.get<int>("time_tmp"));
    std::cout << "Que la partie commence pour la room: " << game->name << std::endl;
    auto end = std::chrono::steady_clock::now();
    while (is_not_finished) {
      std::chrono::duration<double> diff = start - end;

      duration = static_cast<std::size_t>(diff.count() * 1000);
      if (duration > Enum::REFRESH_TIME) {
	end = start;
	is_not_finished = g.update(game, duration);
      }
      start = std::chrono::steady_clock::now();
      if (g.bossIsDead(game) && g.gameTransition(game))
	g.createGame(game);
    }
    game->is_playing = false;
    std::cout << "c'est fini" << std::endl;
}

template <typename SCK>
IServerSocket<SCK>	*GameManager<SCK>::getUDPSocket() { return (_udp_socket); }

template <typename SCK>
void		GameManager<SCK>::sendPosition(Game<SCK> *game, UserManager<SCK> *user) {
  Packet<UDPDataHeader> packet;
  Position		p;
  UDPDataHeader		pack;
  std::string		s;
  std::string		key;
  std::ostringstream	os[2];

  p = user->getPosition();
  os[0] << p.x;
  os[1] << p.y;
  s = user->getName() + ":" + os[0].str() + ":" + os[1].str();
  key = user->getName() + ":" + std::to_string(user->getKeypressed());
  for (auto it = game->players.begin(); it != game->players.end(); ++it) {
    pack = {static_cast<uint16_t>(s.size()),
	    static_cast<uint16_t>(Enum::PLAYER_POS),
	    (*it)->getUDPPacketId()};

      packet.stockOnBuff(pack);
      packet.stockOnBuff(s);
      packet.serialize();
      packet.sendPacket<IServerSocket<SCK> *>(this->_udp_socket, (*it)->getIP(), "1726"); // TODO, no magic string

      pack = {static_cast<uint16_t>(key.size()),
	      static_cast<uint16_t>(Enum::USER_KEYPRESS),
	      (*it)->getUDPPacketId()};

      packet.stockOnBuff(pack);
      packet.stockOnBuff(key);
      packet.serialize();
      packet.sendPacket<IServerSocket<SCK> *>(this->_udp_socket, (*it)->getIP(), "1726"); // TODO, no magic string
  }
}

template <typename SCK>
bool        GameManager<SCK>::isPlaying(const std::string &roomname) {
    Game<SCK> *g = getGameByName(roomname);

    return (g && g->is_playing);
}

template <typename SCK>
void        GameManager<SCK>::setUdpSocket(IServerSocket<SCK> *sock) { _udp_socket = sock; }

template <typename SCK>
bool        GameManager<SCK>::isAllReady(const std::string &roomname) {
    Game<SCK> *g = getGameByName(roomname);

    if (!g)
        return (false);
    for (auto it = g->players.begin(); it != g->players.end(); ++it) {
      if (!(*it)->isReady()) {
	std::cout << (*it)->getName() << " n'est pas ready dans la room: " << roomname << std::endl;
            return (false);
      }
    }
    std::cout << "Tout le monde est ready dans la room: " << roomname << std::endl;
    return (true);
}

template <typename SCK>
const std::list<Game<SCK> *>        &GameManager<SCK>::getGames() const { return (_games); }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
template class GameManager<SOCKET>;
template struct Game<SOCKET>;
#else
template class GameManager<int>;
template struct Game<int>;
#endif
