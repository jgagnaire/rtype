#include "GameManager.hh"
#include "GameController.hh"

template<typename T>
GameController<T>::GameController(std::list<UserManager<T> *> *cl)
        : AController<T>(*cl) {}

template<typename T>
int	            GameController<T>::newData(UserManager<T> *cl) {
    switch (cl->numQuery()) {
        case (Enum::JOIN_GAME_ROOM):
            return (static_cast<int>(joinNamedRoom(cl)));
        case (Enum::CREATE_ROOM):
            return (static_cast<int>(createGameRoom(cl)));
        case (Enum::LEAVE_ROOM):
            return (static_cast<int>(leaveRoom(cl)));
        case (Enum::READY):
            return (static_cast<int>(ready(cl)));
        case (Enum::NOT_READY):
            return (static_cast<int>(notReady(cl)));
        case (Enum::GET_ROOM):
            return (static_cast<int>(getRoomList(cl)));
        case (Enum::LATENCY):
	  return (static_cast<int>(latency(cl)));
    default:
            return (-1);
    }
}

template <typename T>
bool            GameController<T>::latency(UserManager<T> *cl) const {
  cl->writeStruct({0, static_cast<uint16_t>(cl->setLatency())});
  return (true);
}

template <typename T>
void            GameController<T>::sendJSON(UserManager<T> *cl, Game<T> *game) const {
  auto		    &content = game->content_system;
  const std::pair<std::string, Enum::ServerNotification> strs[] =
    {{"bonuses", Enum::SEND_JSON_BONUS},
     {"fires", Enum::SEND_JSON_FIRE},
     {"monsters", Enum::SEND_JSON_MONSTER},
     {"hitboxes", Enum::SEND_JSON_HITBOX},
     {"levels", Enum::SEND_JSON_LEVEL}};

  for (std::size_t i = 0; i < sizeof(strs) / sizeof(strs[0]); ++i) {
    cl->writeStruct({static_cast<uint16_t>(content[strs[i].first].size()),
		static_cast<uint16_t>(strs[i].second)});
    cl->writeMsg(content[strs[i].first]);
  }
}

template <typename T>
bool            GameController<T>::joinNamedRoom(UserManager<T> *cl) const {
    Enum::ServerAnswers		sa = cl->joinNamedRoom();
    GameManager<T>		&g = GameManager<T>::instance();
    Game<T>			*game;

    cl->writeStruct({0, static_cast<uint16_t>(sa)});
    if (sa == Enum::OK) {
      cl->writeStruct({0, Enum::GET_LATENCY});
	game = g.getGameByName(cl->getGameroomName());
	cl->setId(game->getId());
	std::string tmp = cl->getName() + ":" + std::to_string(cl->getId());
        sendJSON(cl, game);
        if (!game)
            return (true);
        for (auto it = game->players.begin(); it != game->players.end(); ++it) {
	  if (cl->getName() != (*it)->getName()) {
	    std::string tmp2 = (*it)->getName() + ":" + std::to_string((*it)->getId());
            cl->writeStruct({static_cast<uint16_t>(tmp2.size()),
		  Enum::PLAYER_JOIN});
            cl->writeMsg(tmp2);
	  }
            (*it)->writeStruct({static_cast<uint16_t>(tmp.size()),
                                Enum::PLAYER_JOIN});
            (*it)->writeMsg(tmp);
        }
    }
    return (true);
}

template <typename T>
inline
bool            GameController<T>::createGameRoom(UserManager<T> *cl) const {
    Enum::ServerAnswers		sa = cl->createGameRoom();
    GameManager<T>		&g = GameManager<T>::instance();

    cl->writeStruct({0, static_cast<uint16_t>(sa)});
    if (sa == Enum::OK) {
      cl->writeStruct({0, Enum::GET_LATENCY});
      std::string tmp = cl->getName() + ":" + std::to_string(cl->getId());
      std::cout << "l'id et le name: " << tmp << std::endl;
      std::cout << cl->getName() << " veut creer "  << cl->getPacketData() << std::endl;
      cl->writeStruct({static_cast<uint16_t>(tmp.size()),
	    Enum::PLAYER_JOIN});
      cl->writeMsg(tmp);
      sendJSON(cl, g.getGameByName(cl->getGameroomName()));
    }
    return (true);
}

template <typename T>
bool            GameController<T>::leaveRoom(UserManager<T> *cl) const {
    GameManager<T>              &g = GameManager<T>::instance();
    Game<T>                     *game;

    game = g.getGameByName(cl->getGameroomName());
    Enum::ServerAnswers         sa = cl->leaveRoom();
    cl->writeStruct({0, static_cast<uint16_t>(sa)});
    if (sa == Enum::OK) {
        if (!game)
            return (true);
        for (auto it = game->players.begin(); it != game->players.end(); ++it) {
            if ((*it)->getName() != cl->getName()) {
                (*it)->writeStruct({static_cast<uint16_t>(cl->getName().size()),
                                    Enum::PLAYER_LEFT});
                (*it)->writeMsg(cl->getName());
            }
        }
    }
    return (true);
}

template <typename T>
bool            GameController<T>::ready(UserManager<T> *cl) const {
  Enum::ServerAnswers sa = cl->ready();
    GameManager<T>              &g = GameManager<T>::instance();
    Game<T>                     *game = g.getGameByName(cl->getGameroomName());

    cl->writeStruct({0, static_cast<uint16_t>(sa)});
    if (sa == Enum::OK) {
      if (g.isPlaying(cl->getGameroomName())) {
        cl->inGame();
        cl->clearGameData();
        cl->writeStruct({0, Enum::GAME_START});
      }
      else if (g.isAllReady(cl->getGameroomName())) {
        if (game) {
	  for (auto it = game->players.begin(); it != game->players.end(); ++it) {
	    std::cout << "je clean: " << (*it)->getName() << std::endl;
	    (*it)->inGame();
	    (*it)->clearGameData();
	    (*it)->writeStruct({0, Enum::GAME_START});
	  }
	  g.launchGame(cl->getGameroomName());
        }
      }
    }
    return (true);
}

template <typename T>
inline
bool            GameController<T>::notReady(UserManager<T> *cl) const {
    cl->writeStruct({0, static_cast<uint16_t>(cl->notReady())});
    return (true);
}

template <typename T>
bool            GameController<T>::getRoomList(UserManager<T> *cl) const {
    GameManager<T>              &g = GameManager<T>::instance();
    Enum::ServerAnswers      sa = cl->getRoomList();
    const std::list<Game<T> *>  &game =  g.getGames();
    std::ostringstream          os;
    std::string                 tmp;

    cl->writeStruct({0, static_cast<uint16_t>(sa)});
    if (sa == Enum::OK) {
      for (auto it = game.begin(); it != game.end(); ++it) {
	if ((*it)->players.size() != 4) {
	  tmp.clear();
	  os.clear();
	  os.str("");
	  os <<  (Enum::MAX_PLAYER - (*it)->players.size());
	  tmp = (*it)->name + ":" + os.str();
	  std::cout << "Nom de room: " << tmp << std::endl;
	  cl->writeStruct({static_cast<uint16_t>(tmp.size()),
		Enum::GAME_NAME});
	  cl->writeMsg(tmp);
	}
      }
    }
    return (true);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
template class GameController<SOCKET>;
#else
template class GameController<int>;
#endif
