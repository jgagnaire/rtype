#include <GameManager.hh>
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
    GameManager<T>             &g = GameManager<T>::instance();
    Enum::TCPServerAnswers  sa = cl->joinRandomRoom();
    Game<T>                    *game;

    cl->writeStruct({0, sa});
    if (sa == Enum::OK) {
        game = g.getGameByName(cl->getGameroomName());
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
    GameManager<T>             &g = GameManager<T>::instance();
    Enum::TCPServerAnswers  sa = cl->joinNamedRoom();
    Game<T>                    *game;

    cl->writeStruct({0, sa});
    if (sa == Enum::OK) {
        game = g.getGameByName(cl->getGameroomName());
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
    cl->writeStruct({0, cl->createGameRoom()});
    return (true);
}

template <typename T>
bool            GameController<T>::leaveRoom(UserManager<T> *cl) const {
    GameManager<T>              &g = GameManager<T>::instance();
    Enum::TCPServerAnswers      sa = cl->leaveRoom();
    Game<T>                     *game;

    cl->writeStruct({0, sa});
    if (sa == Enum::OK) {
        game = g.getGameByName(cl->getGameroomName());
        for (auto it = game->players.begin(); it != game->players.end(); ++it) {
            (*it)->writeStruct({static_cast<uint16_t>(cl->getName().size()),
                                Enum::PLAYER_LEFT});
            (*it)->writeMsg(cl->getName());
        }
    }
    return (true);
}

template <typename T>
bool            GameController<T>::ready(UserManager<T> *cl) const {
    cl->writeStruct({0, cl->ready()});
    return (true);
}

template <typename T>
bool            GameController<T>::notReady(UserManager<T> *cl) const {
    cl->writeStruct({0, cl->notReady()});
    return (true);
}

template <typename T>
bool            GameController<T>::getRoomList(UserManager<T> *cl) const {
    GameManager<T>              &g = GameManager<T>::instance();
    Enum::TCPServerAnswers      sa = cl->getRoomList();
    const std::list<Game<T> *>  &game =  g.getGames();
    std::ostringstream          os;
    std::string                 tmp;

    cl->writeStruct({0, sa});
    if (sa == Enum::OK) {
        for (auto it = game.begin(); it != game.end(); ++it) {
            tmp.clear();
            os.clear();
            os.str("");
            os <<  (4 - (*it)->players.size());
            tmp = (*it)->name + ":" + os.str();
            cl->writeStruct({static_cast<uint16_t>(tmp.size()),
                             Enum::GAME_NAME});
            cl->writeMsg(tmp);
        }
    }
    return (true);
}