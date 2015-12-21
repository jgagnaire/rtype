#pragma once
# include "AController.hh"

template<typename T>
class GameController : public AController<T> {
public:
    GameController(std::list<UserManager<T> *> *);

    virtual         ~GameController() {}
    
    void	    sendJSON(UserManager<T> *, Game<T> *) const;
    bool            joinNamedRoom(UserManager<T> *) const;
    bool            createGameRoom(UserManager<T> *) const;
    bool            latency(UserManager<T> *) const;
    bool            getRoomList(UserManager<T> *) const;
    bool            leaveRoom(UserManager<T> *) const;
    bool            ready(UserManager<T> *) const;
    bool            notReady(UserManager<T> *) const;
    virtual int	    newData(UserManager<T> *);
private:
	GameController	&operator=(GameController const &rhs) { if (&rhs != this) {} return *this; }
};
