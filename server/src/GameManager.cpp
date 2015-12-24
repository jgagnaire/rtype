#include <chrono>
#include "GameManager.hh"
#include "JSONSerializer.hh"

template <typename SCK>
GameManager<SCK> *GameManager<SCK>::game_manager = 0;

template <typename SCK>
Entity GameManager<SCK>::configuration;

template <typename SCK>
DLLoader *GameManager<SCK>::dlloader = 0;

template <typename SCK>
GameManager<SCK>::GameManager() {
  const std::string strs[] = { "fires", "levels", "monsters",
			       "bonuses", "hitboxes" };
# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
  std::string	tmp[2] = {"", "dll"};
  GameManager<SCK>::dlloader = new WinDLLoader;
# else
  std::string	tmp[2] = {"lib", "so"};
  GameManager<SCK>::dlloader = new UnixDLLoader;
#endif
  try {
    for (uint64_t i = 0; i < sizeof(strs) / sizeof(strs[0]); ++i) {
      std::string s = GameManager<SCK>::configuration.manager.get<std::string>(strs[i]);
      s.replace(s.find("%"), 2, tmp[0]);
      s.replace(s.find("%"), 2, tmp[1]);
      GameManager<SCK>::dlloader->openLib(s, strs[i]);
      GameManager<SCK>::dlloader->loadLib(strs[i], "dl_entry_point");
    }
  }
  catch(const std::exception &e) {}
}

template <typename SCK>
GameManager<SCK>     &GameManager<SCK>::instance() {
  if (GameManager::game_manager == 0) {
    JSONParser	*p;

    try {
      JSONParser::parseFile("./entities/configuration.json");
      p = JSONParser::parse();
      GameManager<SCK>::configuration = p->getEntity().manager.get<Entity>("configuration");
    }
    catch (...) { std::cerr << "PARSING ERROR !!" << std::endl; }
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
bool        GameManager<SCK>::createRoom(const std::string &name, UserManager<SCK> *s) {
  Game<SCK>    *g = new Game<SCK>;
  const std::string strs[] = { "fires", "levels", "monsters",
			       "bonuses", "hitboxes" };

  g->name = name;
  g->players.push_back(s);
  try {
    for (uint64_t i = 0; i < sizeof(strs) / sizeof(strs[0]); ++i) {
      Entity *ent = (*GameManager<SCK>::dlloader)(strs[i], "dl_entry_point");
      if (!ent)
	return (false);
      g->entities[strs[i]] = *ent;
      ent->manager.get<Entity>(strs[i]);
      g->content_system[strs[i]] = JSONSerializer::generate(ent->manager.get<Entity>(strs[i]), strs[i]);
      delete ent;
    }
  }
  catch (const std::exception &) { return(false); }
  _games.push_back(g);
  return (true);
}

template <typename SCK>
void        GameManager<SCK>::deleteUser(UserManager<SCK> *u) {
    Game<SCK> *g = getGameByName(u->getGameroomName());

    if (!g)
        return ;
    for (auto it = g->players.begin(); it != g->players.end(); ++it) {
        if (u->getName() == (*it)->getName()) {
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

  if (second_weapon) {
    ent = new Entity(tmp.manager.get<Entity>("fires").manager.get<Entity>("sinusoid"));
    ent->manager.add<uint64_t>("id", game->shoot_player_ids++);
    ent->manager.set<int>("damage", ent->manager.get<int>("damage") * u->getForce());
    game->system["shoot"]->handle("sinusoid", ent, false, u->getPosition());
  }
  else {
    ent = new Entity(tmp.manager.get<Entity>("fires").manager.get<Entity>("line"));
    ent->manager.add<uint64_t>("id", game->shoot_player_ids++);
    ent->manager.set<int>("damage", ent->manager.get<int>("damage") * u->getForce());
    game->system["shoot"]->handle("line", ent, false, u->getPosition());
  }
}

template <typename SCK>
bool        GameManager<SCK>::joinRoom(const std::string &name, UserManager<SCK> *u) {
    if (roomIsFull(name))
        return (false);
    Game<SCK> *game = getGameByName(name);

    if (!game || game->is_playing)
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
void		GameManager<SCK>::setPort(const std::string &p) {
  port = p;
  ASystem::setPort(port);
}

template <typename SCK>
void        GameManager<SCK>::reloadJSON(Game<SCK> *game) {
  const std::pair<std::string, Enum::ServerNotification> strs[] =
    {{"bonuses", Enum::SEND_JSON_BONUS},
     {"fires", Enum::SEND_JSON_FIRE},
     {"monsters", Enum::SEND_JSON_MONSTER},
     {"hitboxes", Enum::SEND_JSON_HITBOX},
     {"levels", Enum::SEND_JSON_LEVEL}};

  cleanSystem(game);
  try {
    for (uint64_t i = 0; i < sizeof(strs) / sizeof(strs[0]); ++i) {
      Entity *ent = (*GameManager<SCK>::dlloader)(strs[i].first, "dl_entry_point");

      if (!ent)
	return ;
      game->entities[strs[i].first] = *ent;
      ent->manager.get<Entity>(strs[i].first);
      game->content_system[strs[i].first] =
	JSONSerializer::generate(ent->manager.get<Entity>(strs[i].first), strs[i].first);
      for (auto cl = game->players.begin(); cl != game->players.end(); ++cl) {
	(*cl)->writeStruct({static_cast<uint16_t>(game->content_system[strs[i].first].size()),
	      static_cast<uint16_t>(strs[i].second)});
	(*cl)->writeMsg(game->content_system[strs[i].first]);
      }
      delete ent;
    }
  }
  catch (...) {}
}

template <typename SCK>
void        GameManager<SCK>::launchGame(const std::string &game_name) {
  Game<SCK>    *game = getGameByName(game_name);

  if (game && !game->is_playing) {
    game->is_playing = true;
    game->lvl_name = "level1";
    _threadpool.startThread(_threadpool.add(&GameManager::createGame, game));
  }
}

template <typename SCK>
void        GameManager<SCK>::updatePositions(Game<SCK> *game, uint64_t time) {
    for (auto it = game->players.begin(); it != game->players.end(); ++it)
        (*it)->changePosition(time);
}

template <typename SCK>
bool        GameManager<SCK>::updateTime(Game<SCK> *game) {
    Entity	&tmp = game->entities["levels"];
    Entity	&entity =
      tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);
    uint64_t	game_time = entity.manager.get<int>("time");
    uint64_t	tmp_time = getTimeInSecond() + entity.manager.get<int>("time_tmp") - game_time;

    entity.manager.set<int>("time_tmp", entity.manager.get<int>("time_tmp") -
			    static_cast<std::size_t>(tmp_time));
    entity.manager.set<int>("timeleft", entity.manager.get<int>("timeleft") +
			    static_cast<std::size_t>(tmp_time));
    return (entity.manager.get<int>("time_tmp") <= 0);
}

template <typename SCK>
void        GameManager<SCK>::fireMob(Game<SCK> *game, Entity *monster,
				      const std::string &monster_name, Position &p) {
  Entity	*ent;
  Entity	&tmp = game->entities["fires"];
  auto		&monster_fires = monster->manager.get<std::vector<std::string> >("fire");

  for (auto fires = monster_fires.begin(); fires != monster_fires.end(); ++fires) {
    if (!(*fires).empty()) {
      ent = new Entity(tmp.manager.get<Entity>("fires").manager.get<Entity>(*fires));
      ent->manager.add<uint64_t>("id", game->shoot_mob_ids++);
      game->system["shoot"]->handle(monster_name, ent, true, p);
    }
  }
}

template <typename SCK>
bool
GameManager<SCK>::checkEntities(Game<SCK> *game,
				std::pair<std::string, Entity&> &entity,
				int time,
				uint64_t duration,
				const std::string &ent_name) {
  Entity	&tmp = game->entities[ent_name];
  Entity	&entity_pos = entity.second.manager.get<Entity>("position");
  Position	pos = {entity_pos.manager.get<float>("x"),
		       entity_pos.manager.get<float>("y")};

  if (time >= entity.second.manager.get<int>("appear_at_sec")) {
    if (entity.second.manager.exist<int>("refresh")) {
      if (ent_name != "bonuses" && entity.second.manager.get<int>("timeval") <=
	  entity.second.manager.get<int>("refresh")) {
	Entity	*ent =
	  new Entity(tmp.manager.get<Entity>(ent_name).manager.get<Entity>(entity.first));
	ent->manager.add<uint64_t>("id", game->monster_ids++);
	entity.second.manager.set<int>("refresh", 0);
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
	ent->manager.add<uint64_t>("id", game->bonus_ids++);
      else
	ent->manager.add<uint64_t>("id", game->monster_ids++);
      game->system[ent_name]->handle(entity.first, ent, true, pos);
      if (entity.second.manager.exist<int>("time")) {
	entity.second.manager.set<int>("time",
				       entity.second.manager.get<int>("time") - 1);
      }
      else
	return (true);
    }
  }
  if (entity.second.manager.exist<int>("time")) {
    return (entity.second.manager.get<int>("time") <= 0);
  }
  return (false);
}

template <typename SCK>
bool        GameManager<SCK>::updateObjSighting(Game<SCK> *game, uint64_t time,
						     const std::string &ent_name) {
  Entity	&tmp = game->entities["levels"];
  Entity	&entity =
    tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);
  std::vector<Entity>  &objs = entity.manager.get<std::vector<Entity> >(ent_name);
  std::vector<Entity>  tmp_objs;

  objs.erase(std::remove_if(objs.begin(), objs.end(), [&](Entity &e) {
	auto m = e.manager.getAll<Entity>();
	std::pair<std::string, Entity&> tmp_obj = {m[0].first,
						   e.manager.get<Entity>(m[0].first)};
	if (!(m.empty() || checkEntities(game, tmp_obj,
					 entity.manager.get<int>("timeleft"), time, ent_name)))
	  tmp_objs.push_back(e);
	return (false);
      }), objs.end());
  entity.manager.remove<std::vector<Entity> >(ent_name);
  entity.manager.add<std::vector<Entity> >(ent_name, std::vector<Entity>());
  std::vector<Entity>  &stock = entity.manager.get<std::vector<Entity> >(ent_name);
  for (auto obj = tmp_objs.begin();
       obj != tmp_objs.end(); ++obj)
    stock.push_back(*obj);
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
  ent->manager.add<uint64_t>("id", game->boss_ids++);
  std::cout << "Que le boss " << boss_name << " arrive !!" << std::endl;
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
bool        GameManager<SCK>::update(Game<SCK> *game, uint64_t time) {
  if (updateTime(game))
    updateBoss(game);
  updatePositions(game, time);
  updateObjSighting(game, time, "monsters");
  updateObjSighting(game, time, "bonuses");
  game->system["shoot"]->update(static_cast<std::size_t>(time), game);
  game->system["monsters"]->update(static_cast<std::size_t>(time), game);
  game->system["bonuses"]->update(static_cast<std::size_t>(time), game);
  game->system["boss"]->update(static_cast<std::size_t>(time), game);
  for (auto p = game->players.begin(); p != game->players.end(); ++p)
    if (!(*p)->isDead())
      (*p)->updateBonus(time);
  ASystem::collision(game->system, game->players, game->entities);
  return (!(game->players.empty() ||
	    isAllDead(game) || bossIsDead(game)));
}

template <typename SCK>
uint64_t   GameManager<SCK>::getTimeInSecond() {
    return (static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch() /
           std::chrono::seconds(1)));
}

template <typename SCK>
uint64_t   GameManager<SCK>::getTime() {
    return (static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1)));
}

template <typename SCK>
void		GameManager<SCK>::cleanSystem(Game<SCK> *game) {
  game->system["shoot"]->clear();
  game->system["monsters"]->clear();
  game->system["bonuses"]->clear();
  game->system["boss"]->clear();
  game->shoot_player_ids = Enum::MAX_ID;
  game->monster_ids = 2 * Enum::MAX_ID;
  game->bonus_ids = 3 * static_cast<uint64_t>(Enum::MAX_ID);
  game->shoot_mob_ids = 4 * static_cast<uint64_t>(Enum::MAX_ID);
  game->boss_ids = 5 * static_cast<uint64_t>(Enum::MAX_ID);
}

template <typename SCK>
bool            GameManager<SCK>::gameTransition(Game<SCK> *game) {
  uint64_t	time = GameManager<SCK>::getTimeInSecond() + 10;
  char		lvl = game->lvl_name.back();

  cleanSystem(game);
  if (lvl >= '5')
    return (false);
  game->lvl_name.pop_back();
  game->lvl_name.push_back(lvl + 1);
  for (auto p = game->players.begin(); p != game->players.end(); ++p)
    (*p)->clearLevel();
  std::cout << "le " << game->lvl_name << " va bientot commencer..." << std::endl;
  for(; time > GameManager<SCK>::getTimeInSecond(););
  return (true);
}

template <typename SCK>
void            GameManager<SCK>::createGame(Game<SCK> *game) {
  try {
    if (game->lvl_name == "level1") {
      uint64_t	time = GameManager<SCK>::getTime() + 500;
      for(; time > GameManager<SCK>::getTime(););
    }
    GameManager<SCK>	&g = GameManager<SCK>::instance();
    bool		is_not_finished = true;
    uint64_t		duration;
    auto		start = std::chrono::steady_clock::now();
    Entity		&tmp = game->entities["levels"];
    Entity		&tmp_entity =
      tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);

    std::cout << "C'est partie pour le niveau " << game->lvl_name << std::endl;
    tmp_entity.manager.add<int>("time_tmp", tmp_entity.manager.get<int>("time"));
    tmp_entity.manager.add<int>("timeleft", 0);
    tmp_entity.manager.set<int>("time", static_cast<int>(getTimeInSecond()) +
				tmp_entity.manager.get<int>("time_tmp"));
    std::cout << "Que la partie commence pour la room: " << game->name << std::endl;
    auto end = std::chrono::steady_clock::now();
    while (is_not_finished) {
      std::chrono::duration<double> diff = start - end;

      duration = static_cast<uint64_t>(diff.count() * 1000);
      if (duration > Enum::REFRESH_TIME) {
	end = start;
	is_not_finished = g.update(game, duration);
      }
      start = std::chrono::steady_clock::now();
      if (g.bossIsDead(game) && g.gameTransition(game))
	return createGame(game);
    }
    for (auto p = game->players.begin(); p != game->players.end(); ++p) {
      (*p)->writeStruct({0, static_cast<uint16_t>(Enum::GAME_END)});
      (*p)->onGameRoom();
    }
    g.reloadJSON(game);
    std::cout << "Pour la room: " << game->name << ", c'est fini" << std::endl;
    game->is_playing = false;
  }
  catch (...) {
    std::cerr << "AWWW... erreur critique pour la room: " << game->name << std::endl;
    for (auto p = game->players.begin(); p != game->players.end(); ++p) {
      (*p)->writeStruct({0, static_cast<uint16_t>(Enum::GAME_ERROR)});
      (*p)->onLobby();
    }
    game->players.clear();
  }
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
      packet.sendPacket<IServerSocket<SCK> *>(this->_udp_socket, (*it)->getIP(), port);

      pack = {static_cast<uint16_t>(key.size()),
	      static_cast<uint16_t>(Enum::USER_KEYPRESS),
	      (*it)->getUDPPacketId()};

      packet.stockOnBuff(pack);
      packet.stockOnBuff(key);
      packet.serialize();
      packet.sendPacket<IServerSocket<SCK> *>(this->_udp_socket, (*it)->getIP(), port);
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
const std::list<Game<SCK> *>        &GameManager<SCK>::getGames() {
  for (auto it = _games.begin(); it != _games.end();) {
    if ((*it)->players.size() == 0) {
      it = _games.erase(it);
      break ;
    }
    else
      ++it;
  }
  return (_games);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
template class GameManager<SOCKET>;
template struct Game<SOCKET>;
#else
template class GameManager<int>;
template struct Game<int>;
#endif
