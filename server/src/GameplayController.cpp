#include "GameManager.hh"
#include "GameplayController.hh"

template<typename T>
GameplayController<T>::GameplayController(std::list<UserManager<T> *> *cl)
: AController<T>(*cl) {}

template<typename T>
int	            GameplayController<T>::newData(UserManager<T> *cl) {
    if (!cl->isLogged()) {
      std::cout << "that" << std::endl;
        this->writeStruct({0, Enum::EUDP_NOT_LOGGEDIN, cl->getUDPPacketId()});
        this->writeOnMe(cl->getIP()); 
        return (1);
    }
    else if (cl->getStatus() != Enum::GAME_ROOM) {
      std::cout << "this" << std::endl;
        this->writeStruct({0, Enum::ENOT_IN_GAME, cl->getUDPPacketId()});
        this->writeOnMe(cl->getIP()); 
        return (1);
    }
    std::cout << "other" << std::endl;
    switch (cl->numUDPQuery()) {
        case (Enum::QUIT_GAME):
            return (static_cast<int>(quitGame(cl)));
        case (Enum::CURRENT_POS):
            return (static_cast<int>(currentPosition(cl)));
        case (Enum::KEY_PRESS):
            return (static_cast<int>(keyPressed(cl)));
        case (Enum::AUDIO_PACKET):
            return (static_cast<int>(audioPacket(cl)));
        case (Enum::TAKE_FORCE):
            return (static_cast<int>(takeForce(cl)));
        default:
            return (-1);
    }
}

template <typename T>
bool            GameplayController<T>::quitGame(UserManager<T> *cl) {
    GameManager<T>     &g = GameManager<T>::instance();
    Game<T>           *game = g.getGameByName(cl->getGameroomName());

    if (game) {
        this->writeStruct({0, static_cast<uint16_t>(cl->quitGame()), cl->getUDPPacketId()});
        this->writeOnMe(cl->getIP()); 
        const std::string &u_name = cl->getName();
        for (auto it = game->players.begin(); it != game->players.end(); ++it) {
            if ((*it)->getName() != u_name) {
                this->writeStruct({static_cast<uint16_t>(u_name.size()),
                             Enum::USER_QUIT_GAME, (*it)->getUDPPacketId()});
               this->writeMsg(u_name);
               this->writeOnMe((*it)->getIP()); 
            }
        }
        g.launchGame(cl->getGameroomName());
    }
    cl->writeStruct({0, static_cast<uint16_t>(cl->quitGame())});
    return (true);
}

template <typename T>
inline
bool            GameplayController<T>::currentPosition(UserManager<T> *cl) {
    this->writeStruct({0, static_cast<uint16_t>(cl->currentPosition()), cl->getUDPPacketId()});
    this->writeOnMe(cl->getIP());
    return (true);
}

template <typename T>
inline
bool            GameplayController<T>::keyPressed(UserManager<T> *cl) {
    this->writeStruct({0, static_cast<uint16_t>(cl->keyPressed()), cl->getUDPPacketId()});
    this->writeOnMe(cl->getIP()); 
    return (true);
}

template <typename T>
inline
bool            GameplayController<T>::audioPacket(UserManager<T> *) const {
    return (true);
}

template <typename T>
inline
bool            GameplayController<T>::takeForce(UserManager<T> *cl) {
    this->writeStruct({0, static_cast<uint16_t>(cl->takeForce()), cl->getUDPPacketId()});
    this->writeOnMe(cl->getIP()); 
    return (true);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
template class GameplayController<SOCKET>;
#else
template class GameplayController<int>;
#endif
