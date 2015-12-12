#include "GameManager.hh"
#include "GameController.hh"

template<typename T>
GameController<T>::GameController(std::list<UserManager<T> *> *cl)
        : AController<T>(*cl) {}

template<typename T>
int	            GameController<T>::newData(UserManager<T> *cl) {
    switch (cl->numQuery()) {
        case (Enum::JOIN_RANDOM_ROOM):
            return (static_cast<int>(joinRandomRoom(cl)));
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
        default:
            return (-1);
    }
}

template <typename T>
bool            GameController<T>::joinRandomRoom(UserManager<T> *cl) const { //TODO, refractor
    GameManager<T>              &g = GameManager<T>::instance();
    Enum::ServerAnswers         sa = cl->joinRandomRoom();
    Game<T>                     *game;

    cl->writeStruct({0, static_cast<uint16_t>(sa)});
    if (sa == Enum::OK) {
        game = g.getGameByName(cl->getGameroomName());
        if (!game)
            return (true);
        for (auto it = game->players.begin(); it != game->players.end(); ++it) {
            cl->writeStruct({static_cast<uint16_t>((*it)->getName().size()),
                             Enum::PLAYER_JOIN});
            cl->writeMsg((*it)->getName());
            (*it)->writeStruct({static_cast<uint16_t>(cl->getName().size()),
                                Enum::PLAYER_JOIN});
            (*it)->writeMsg(cl->getName());
        }
    }
    return (true);
}

template <typename T>
bool            GameController<T>::joinNamedRoom(UserManager<T> *cl) const {
    GameManager<T>              &g = GameManager<T>::instance();
    Enum::ServerAnswers      sa = cl->joinNamedRoom();
    Game<T>                     *game;

    cl->writeStruct({0, static_cast<uint16_t>(sa)});
    if (sa == Enum::OK) {
        game = g.getGameByName(cl->getGameroomName());
        if (!game)
            return (true);
        for (auto it = game->players.begin(); it != game->players.end(); ++it) {
            cl->writeStruct({static_cast<uint16_t>((*it)->getName().size()),
                             Enum::PLAYER_JOIN});
            cl->writeMsg((*it)->getName());
            (*it)->writeStruct({static_cast<uint16_t>(cl->getName().size()),
                                Enum::PLAYER_JOIN});
            (*it)->writeMsg(cl->getName());
        }
    }
    return (true);
}

template <typename T>
bool            GameController<T>::createGameRoom(UserManager<T> *cl) const {
    std::cout << cl->getName() << " veut creer "  << cl->getPacketData() << std::endl;
    cl->writeStruct({0, static_cast<uint16_t>(cl->createGameRoom())});
    return (true);
}

template <typename T>
bool            GameController<T>::leaveRoom(UserManager<T> *cl) const {
    GameManager<T>              &g = GameManager<T>::instance();
    Enum::ServerAnswers         sa = cl->leaveRoom();
    Game<T>                     *game;

    game = g.getGameByName(cl->getGameroomName());
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
    GameManager<T>              &g = GameManager<T>::instance();
    Game<T>                     *game = g.getGameByName(cl->getGameroomName());

    cl->writeStruct({0, static_cast<uint16_t>(cl->ready())});
    if (g.isPlaying(cl->getGameroomName())) {
        cl->inGame();
        cl->clearGameData();
        cl->writeStruct({0, Enum::GAME_START});
    }
    else if (g.isAllReady(cl->getGameroomName())) {
        if (game) {
            for (auto it = game->players.begin(); it != game->players.end(); ++it) {
                (*it)->inGame();
                (*it)->clearGameData();
                (*it)->writeStruct({0, Enum::GAME_START});
            }
            g.launchGame(cl->getGameroomName());
        }
    }
    return (true);
}

template <typename T>
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
    std::cout << "qwe " << sa << std::endl;
    std::cout << "la taille des games: " << game.size() << std::endl;
    if (sa == Enum::OK) {
        for (auto it = game.begin(); it != game.end(); ++it) {
            tmp.clear();
            os.clear();
            os.str("");
            os <<  (Enum::MAX_PLAYER - (*it)->players.size());
            tmp = (*it)->name + ":" + os.str();
            std::cout << "LOL: " << tmp << std::endl;
            cl->writeStruct({static_cast<uint16_t>(tmp.size()),
                             Enum::GAME_NAME});
            cl->writeMsg(tmp);
        }
    }
    return (true);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
template class GameController<SOCKET>;
#else
template class GameController<int>;
#endif
