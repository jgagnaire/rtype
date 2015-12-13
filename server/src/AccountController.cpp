#include "GameManager.hh"
#include "GameController.hh"
#include "AccountController.hh"

template<typename T>
AccountController<T>::AccountController(std::list<UserManager<T> *> *cli)
        : AController<T>(*cli) {}

template<typename T>
int	AccountController<T>::newData(UserManager<T> *cli) {
    cli->fillPacketStruct();
    std::cout << "La query: " << cli->numQuery() << std::endl;
    switch (cli->numQuery()) {
        case Enum::ACCOUNT_LOGIN:
            return (static_cast<int>(accountLoginFct(cli)));
        case Enum::ACCOUNT_REGISTER:
            return (static_cast<int>(accountRegisterFct(cli)));
        case Enum::DISCONNECTION:
            return (static_cast<int>(disconnectionFct(cli)));
        case Enum::PONG:
            return (static_cast<int>(pong(cli)));
        default:
            return (-1);
    }
}

template<typename T>
inline
bool    AccountController<T>::pong(UserManager<T> *cl) const {
    cl->setPing(true);
    return (true);
}

template<typename T>
inline
int    AccountController<T>::newConnection(UserManager<T> *cl) const {
    cl->sendPing();
    return (1);
}

template<typename T>
inline
bool    AccountController<T>::accountLoginFct(UserManager<T> *cl) const {
    cl->writeStruct({0, static_cast<uint16_t>(cl->verifyUser())});
    return (true);
}

template<typename T>
inline
bool    AccountController<T>::accountRegisterFct(UserManager<T> *cl) const {
    cl->writeStruct({0, static_cast<uint16_t>(cl->newUser())});
    return (true);
}

template<typename T>
inline
bool	AccountController<T>::disconnectionFct(UserManager<T> *cl) const {
    GameManager<T>              &g = GameManager<T>::instance();
    Game<T>                     *game;

    game = g.getGameByName(cl->getGameroomName());
    if (!cl->isLogged())
        return (false);
    if (!game)
      return (true);
    for (auto it = game->players.begin(); it != game->players.end(); ++it) {
      if ((*it)->getName() != cl->getName()) {
	(*it)->writeStruct({static_cast<uint16_t>(cl->getName().size()),
	      Enum::PLAYER_LEFT});
	(*it)->writeMsg(cl->getName());
      }
    }
    cl->disconnect();
    return (false);
}

template<typename T>
inline
int    AccountController<T>::closeConnection(UserManager<T> *cl) const {
    disconnectionFct(cl);
    return (0);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
template class AccountController<SOCKET>;
#else
template class AccountController<int>;
#endif
