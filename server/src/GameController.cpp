#include "GameController.hh"

template<typename T>
GameController<T>::GameController(std::list<UserManager<T> *> *cli)
        : AController<T>(*cli) {}

template<typename T>
int	            GameController<T>::newData(UserManager<T> *cli) {
    switch (cli->numQuery()) {
        case (Enum::JOIN_RANDOM_ROOM):
            return (static_cast<int>(joinRandomRoom(cli)));
        case (Enum::JOIN_GAME_ROOM):
            return (static_cast<int>(joinNamedRoom(cli)));
        case (Enum::CREATE_ROOM):
            return (static_cast<int>(createGameRoom(cli)));
        case (Enum::LEAVE_ROOM):
            return (static_cast<int>(leaveRoom(cli)));
        case (Enum::READY):
            return (static_cast<int>(ready(cli)));
        case (Enum::NOT_READY):
            return (static_cast<int>(notReady(cli)));
        default:
            return (-1);
    }
}

template <typename T>
bool            GameController<T>::joinRandomRoom(UserManager<T> *cli) const {
    return (true);
}

template <typename T>
bool            GameController<T>::joinNamedRoom(UserManager<T> *cli) const {
    return (true);
}

template <typename T>
bool            GameController<T>::createGameRoom(UserManager<T> *cli) const {
    return (true);
}

template <typename T>
bool            GameController<T>::leaveRoom(UserManager<T> *cli) const {
    return (true);
}

template <typename T>
bool            GameController<T>::ready(UserManager<T> *cli) const {
    return (true);
}

template <typename T>
bool            GameController<T>::notReady(UserManager<T> *cli) const {
    return (true);
}