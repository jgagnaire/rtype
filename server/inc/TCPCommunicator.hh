#ifndef RTYPE_TCPCOMMUNICATOR_H
#define RTYPE_TCPCOMMUNICATOR_H

# include "ACommunicator.hh"

template <typename USER, typename CONTROLLER,
        typename MONITOR, typename SCK>
class TCPCommunicator : public ICommunicator<USER, CONTROLLER, MONITOR, SCK> {
private:
    ISocketSet<SCK>		    *srvset;
    typedef bool	(TCPCommunicator::*monitor_ptr)(USER *);
    std::vector<monitor_ptr> monitor_action;
public:
    TCPCommunicator() {
        monitor_action.push_back(&TCPCommunicator::readAction);
        monitor_action.push_back(&TCPCommunicator::writeAction);
        monitor_action.push_back(&TCPCommunicator::acceptAction);
        monitor_action.push_back(&TCPCommunicator::connectAction);
        monitor_action.push_back(&TCPCommunicator::closeAction);
    }
    virtual void	launch() {
        for (;;) {
            for (auto cli = cl_list.begin(); cli != cl_list.end();) {
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                while (!(*cli)->sendStructEmpty()) {
	 if (!(*cli)->writeOnMe()) {
	    deleteClient(cli);
	    break;
	  }

	}
#else
    if (!(*cli)->sendStructEmpty()) {
        this->network_monitor->setObserver((*cli)->getServerSocket(),
                                           static_cast<Enum::Flag>(Enum::READ |
                                                                   Enum::WRITE |
                                                                   Enum::CLOSE));
    }
#endif
                ++cli;
            }
            this->network_monitor->setTimeval(1, 0);
            if ((this->network_monitor->observerFds() == 0) &&
                (timeout))
                timeoutAction();
            else
                checkObserver();
        }
    }
};

void	deleteClient(typename std::list<USER *>::iterator &cli) {
    this->network_monitor->closeFd((*cli)->getServerSocket());
    ((controllers.front())->*closeConnection)(*cli);
    delete *cli;
    *cli = NULL;
    cl_list.erase(cli);
}

void	checkObserver() {
    if (this->network_monitor->isObserved(dynamic_cast<IServerSocket<SCK>*>(this->srvset),
                                          Enum::ACCEPT)) {
        IServerSocket<SCK>	*tmp = this->srvset->absAcceptNewClient();

        if (!tmp)
            return ;
        cl_list.push_back(new UserManager<SCK>(tmp));
        if (newConnection)
            for (auto it = controllers.begin(); it != controllers.end(); ++it)
                ((*it)->*newConnection)(cl_list.back());
        this->network_monitor->addFd(dynamic_cast<IServerSocket<SCK>*>(cl_list.back()->getServerSocket()),
                                     static_cast<Enum::Flag>(Enum::READ | Enum::WRITE | Enum::CLOSE));
        return ;
    }
    for (auto cli = cl_list.begin(); cli != cl_list.end(); ++cli) {
        for (int f = 0; (1 << f) <= Enum::CLOSE; ++f) {
            if (this->network_monitor->isObserved((*cli)->getServerSocket(),
                                                  static_cast<Enum::Flag>(1 << f))) {
                if (!(this->*monitor_action[f])(*cli))
                    return (deleteClient(cli));
            }
        }
    }
    if (aobs)
        afterObserveAction();
}

#endif
