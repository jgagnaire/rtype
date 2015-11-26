#pragma once
# include "AController.hh"

template<typename T>
class GameController : public AController<T> {
    GameController(std::list<UserManager<T> *> *);

    virtual         ~GameController() {}

    bool            joinRandomRoom(UserManager<T> *) const;
    bool            joinNamedRoom(UserManager<T> *) const;
    bool            createGameRoom(UserManager<T> *) const;
    bool            leaveRoom(UserManager<T> *) const;
    bool            ready(UserManager<T> *) const;
    bool            notReady(UserManager<T> *) const;
    virtual int	    newData(UserManager<T> *);
};

