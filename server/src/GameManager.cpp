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
    return (true);
}


template <typename SCK>
void
GameManager<SCK>::checkMonsters(Game<SCK> *game,
				std::pair<std::string, Entity> monster,
				int time,
				std::size_t duration) {
  Entity	&tmp = game->entities["monsters"];
  Entity	monster_pos = monster.second.manager.get<Entity>("position");
  Position	pos = {monster_pos.manager.get<float>("x"),
		       monster_pos.manager.get<float>("y")};

  if (time > monster.second.manager.get<int>("appear_at_sec")) {
    if (monster.second.manager.exist<int>("refresh")) {
      if (monster.second.manager.get<int>("appear_at_sec") <=
	  monster.second.manager.get<int>("refresh")) {
	Entity	*ent =
	  new Entity(tmp.manager.get<Entity>("monsters").manager.get<Entity>(monster.first));
	monster.second.manager.set<int>("refresh", time);
	monster.second.manager.set<int>("time", monster.second.manager.get<int>("time") - 1);
	game->system["monsters"]->handle(monster.first, ent, true, pos);
      }
      else
	monster.second.manager.set<int>("refresh",
					monster.second.manager.get<int>("refresh")
					+ duration);
    }
    else {
      Entity	*ent =
	new Entity(tmp.manager.get<Entity>("monsters").manager.get<Entity>(monster.first));
      monster.second.manager.add<int>("refresh", duration);
      monster.second.manager.set<int>("time",
				      monster.second.manager.get<int>("time") - 1);
      game->system["monsters"]->handle(monster.first, ent, true, pos);
    }
  }
}

template <typename SCK>
bool        GameManager<SCK>::updateMonstersSighting(Game<SCK> *game, std::size_t time) {
  Entity	&tmp = game->entities["levels"];
  Entity	&entity =
    tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);
  auto		&monsters = entity.manager.get<std::vector<Entity> >("monsters");

  for (auto monster = monsters.begin(); monster != monsters.end(); ++monster) {
    auto m = monster->manager.getAll<Entity>();
    if (!m.empty())
      checkMonsters(game, m[0], entity.manager.get<int>("timeleft"), time);
  }
  return (true);
}

template <typename SCK>
bool        GameManager<SCK>::update(Game<SCK> *game, std::size_t time) {
    updateTime(game);
    updatePositions(game, time);
    game->system["shoot"]->update(time);
    return (!game->players.empty());
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
void            GameManager<SCK>::createGame(Game<SCK> *game) {
    GameManager<SCK>	&g = GameManager<SCK>::instance();
    bool		is_not_finished = true;
    std::size_t		duration;
    auto		start = std::chrono::steady_clock::now();
    Entity		&tmp = game->entities["levels"];
    Entity		&tmp_entity =
      tmp.manager.get<Entity>("levels").manager.get<Entity>(game->lvl_name);

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
    }
    game->is_playing = false;
}

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
