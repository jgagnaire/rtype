#ifndef RTYPE_UDPCOMMUNICATOR_H
# define RTYPE_UDPCOMMUNICATOR_H

# include "IServerSocket.hh"
# include "ACommunicator.hh"

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  include "WinUDPSocketSet.hh"
#  include "WinServerMonitor.hh"
# else
#  include "UnixUDPSocketSet.hh"
#  include "UnixServerMonitor.hh"
# endif

template <typename USER, typename CONTROLLER,
            typename MONITOR, typename SCK>
class UDPCommunicator : public ACommunicator<USER, CONTROLLER, MONITOR, SCK> {
public:
    UDPCommunicator(char *port) {
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        this->srvset = new WinUDPSocketSet(!port ? 1725 : std::atoi(port));
        this->network_monitor = new WinServerMonitor();
#else
        this->srvset = new UnixUDPSocketSet(!port ? 1725 : std::atoi(port));
        this->network_monitor = new UnixServerMonitor();
#endif
        this->network_monitor->addFd(dynamic_cast<IServerSocket<SCK>*>(this->srvset),
                                     static_cast<Enum::Flag>(Enum::READ | Enum::WRITE));
    }

    virtual void launch(std::list<USER*> *cl) {
        cl_list = cl;

    }

    virtual bool	writeAction(USER *) { return (true); }
    virtual bool	readAction(USER *cli) { return (true); }
};


#endif
