#pragma once
# include "AController.hh"

template<typename T>
class GameplayController : AController<T> {
public:
    GameplayController(std::list<UserManager<T> *> *);

    virtual         ~GameplayController() {}
    virtual int	    newData(UserManager<T> *);

    bool            quitGame(UserManager<T> *);
    bool            currentPosition(UserManager<T> *);
    bool            keyPressed(UserManager<T> *);
    bool            audioPacket(UserManager<T> *) const;
    bool            takeForce(UserManager<T> *);

private:
    Packet<UDPDataHeader>					packet;

    void	writeStruct(const UDPDataHeader &comdata);
    void	writeMsg(const std::string &);
    bool	writeOnMe(const std::string &, const std::string &);
    GameplayController	&operator=(GameplayController const &) { return *this; }
};

