#ifndef NETWORKMANAGER_H_
# define NETWORKMANAGER_H_

# include <cstdlib>
# include <list>
# include <vector>
# include "ThreadPool.hh"
# include "TCPCommunicator.hh"
# include "UDPCommunicator.hh"
# include "UserManager.hh"
# include "Enum.hh"

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  include "WinServerMonitor.hh"
#  include "WinServerSocket.hh"
#  include "WinTCPSocketSet.hh"
#  include "WinUDPSocketSet.hh"
# else
#  include "UnixServerMonitor.hh"
#  include "UnixServerSocket.hh"
#  include "UnixTCPSocketSet.hh"
#  include "UnixUDPSocketSet.hh"
# endif

template <typename SCK, typename CONTROLLER>
class NetworkManager
{
private:
    std::list<UserManager<SCK> *>	            cl_list;
    IThreadPool<void, std::vector<void *> >     *com_thread;
    ACommunicator<UserManager<SCK>, CONTROLLER, IServerMonitor<SCK>, SCK>   *com[2];
    std::vector<void *>                         arg;
public:
    typedef int   (CONTROLLER::*trigger)(UserManager<SCK> *) const;
    typedef int   (CONTROLLER::*triggerNew)(UserManager<SCK> *);
    typedef void  (CONTROLLER::*Timeout)(std::list<UserManager<SCK> *> &, IServerMonitor<SCK> *) const;
    typedef void  (CONTROLLER::*AfterObserve)(std::list<UserManager<SCK> *> &, IServerMonitor<SCK> *) const;

    NetworkManager(char *port, Enum::Protocol p = Enum::TCP) {
        com[0] = 0;
        com[1] = 0;
        com_thread = new ThreadPool<void, std::vector<void *> >();
        if (static_cast<int>(Enum::TCP) & static_cast<int>(p)) {
            arg.push_back(this);
            com[Enum::TCP_INDEX] = new TCPCommunicator<UserManager<SCK>, CONTROLLER,
                    IServerMonitor<SCK>, SCK>(port);
            arg.push_back(com[Enum::TCP_INDEX]);
            com_thread->add(&ACommunicator<UserManager<SCK>, CONTROLLER, IServerMonitor<SCK>, SCK>::_launch, arg);
        }
        if (static_cast<int>(Enum::UDP) & static_cast<int>(p)) {
            arg.clear();
            arg.push_back(this);
            com[Enum::UDP_INDEX] = new UDPCommunicator<UserManager<SCK>, CONTROLLER,
                    IServerMonitor<SCK>, SCK>(port);
            arg.push_back(com[Enum::UDP_INDEX]);
            com_thread->add(&ACommunicator<UserManager<SCK>, CONTROLLER, IServerMonitor<SCK>, SCK>::_launch, arg);
        }
    }

    void	triggerConnection(trigger connection) { com[Enum::TCP_INDEX]->triggerConnection(connection); }

    void	triggerNewData(triggerNew data) {
        if (com[Enum::TCP])
            com[Enum::TCP_INDEX]->triggerNewData(data);
        if (com[Enum::UDP])
            com[Enum::UDP_INDEX]->triggerNewData(data);
    }

    void	triggerClose(trigger close, Enum::Protocol p = Enum::TCP) {
        if (static_cast<int>(Enum::TCP) & static_cast<int>(p))
            com[Enum::TCP_INDEX]->triggerClose(close);
        if (static_cast<int>(Enum::UDP) & static_cast<int>(p))
            com[Enum::UDP_INDEX]->triggerClose(close);
    }

    void	triggerTimeout(Timeout t, Enum::Protocol p = Enum::TCP) {
        if (static_cast<int>(Enum::TCP) & static_cast<int>(p))
            com[Enum::TCP_INDEX]->triggerTimeout(t);
        if (static_cast<int>(Enum::UDP) & static_cast<int>(p))
            com[Enum::UDP_INDEX]->triggerTimeout(t);
    }

    void	triggerAObserve(AfterObserve ao, Enum::Protocol p = Enum::TCP) {
        if (static_cast<int>(Enum::TCP) & static_cast<int>(p))
            com[Enum::TCP_INDEX]->triggerAObserve(ao);
        if (static_cast<int>(Enum::UDP) & static_cast<int>(p))
            com[Enum::UDP_INDEX]->triggerAObserve(ao);
    }

    void addController(CONTROLLER *c, Enum::Protocol p = Enum::TCP) {
        if (static_cast<int>(Enum::TCP) & static_cast<int>(p))
            com[Enum::TCP_INDEX]->addController(c);
        if (static_cast<int>(Enum::UDP) & static_cast<int>(p))
            com[Enum::UDP_INDEX]->addController(c);
    }

    void	launch() {
        com_thread->startAll();
        com_thread->joinAll();
    }

    inline std::list<UserManager<SCK> *> *getClientList() { return (&this->cl_list); }
    ~NetworkManager() {}
};

#endif // !NETWORKMANAGER_H_