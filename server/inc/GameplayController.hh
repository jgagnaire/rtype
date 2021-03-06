#pragma once
# include "AController.hh"

template<typename T>
class GameplayController : public AController<T> {
public:
    GameplayController(std::list<UserManager<T> *> *);

    virtual         ~GameplayController() {}
    virtual int	    newData(UserManager<T> *);

    bool            quitGame(UserManager<T> *);
    bool            keyPressed(UserManager<T> *);
    bool            audioPacket(UserManager<T> *);

private:
    GameplayController	&operator=(GameplayController const &) { return *this; }
};

