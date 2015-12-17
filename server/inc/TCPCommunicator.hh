#ifndef RTYPE_TCPCOMMUNICATOR_H
#define RTYPE_TCPCOMMUNICATOR_H

# include "ACommunicator.hh"

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  include "WinServerMonitor.hh"
#  include "WinTCPSocketSet.hh"
# else
#  include "UnixServerMonitor.hh"
#  include "UnixTCPSocketSet.hh"
# endif

template <typename USER, typename CONTROLLER,
        typename MONITOR, typename SCK>
class TCPCommunicator : public ACommunicator<USER, CONTROLLER, MONITOR, SCK> {
private:
    typedef bool	(TCPCommunicator::*monitor_ptr)(USER *);
    std::vector<monitor_ptr> monitor_action;
public:
    TCPCommunicator(char *port) {
        monitor_action.push_back(&TCPCommunicator::readAction);
        monitor_action.push_back(&TCPCommunicator::writeAction);
        monitor_action.push_back(&TCPCommunicator::acceptAction);
        monitor_action.push_back(&TCPCommunicator::connectAction);
        monitor_action.push_back(&TCPCommunicator::closeAction);
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        this->srvset = new WinTCPSocketSet(!port ? 1119 : std::atoi(port));
        this->network_monitor = new WinServerMonitor();
#else
        this->srvset = new UnixTCPSocketSet(!port ? 1119 : std::atoi(port));
        this->network_monitor = new UnixServerMonitor();
#endif
        this->network_monitor->addFd(dynamic_cast<IServerSocket<SCK>*>(this->srvset),
                                     static_cast<Enum::Flag>(Enum::ACCEPT));
    }

    virtual ~TCPCommunicator() {}

    virtual void	launch(std::list<USER*> *clients) {
        this->cl_list = clients;
        for (;;) {
            for (auto cli = this->cl_list->begin(); cli != this->cl_list->end();) {
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
                while (!(*cli)->sendStructEmpty()) {
	    if (!(*cli)->writeOnMe()) {
			std::cout << "je supprime: " << (*cli)->getName() << std::endl;
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
                (this->timeout))
                this->timeoutAction();
            else
                this->checkObserver();
        }
    }

    void	deleteClient(typename std::list<USER *>::iterator &cli) {
        UserManager<SCK>::user_mutex->lock();
        this->network_monitor->closeFd((*cli)->getServerSocket());
        ((this->controllers.front())->*(this->closeConnection))(*cli);
        delete *cli;
        *cli = 0;
        this->cl_list->erase(cli);
	UserManager<SCK>::user_mutex->unlock();
    }

    void	checkObserver() {
        if (this->network_monitor->isObserved(dynamic_cast<IServerSocket<SCK>*>(this->srvset),
                                              Enum::ACCEPT)) {
            std::cout << "Il y a un nouveau client !!" << std::endl;
            IServerSocket<SCK>	*tmp = this->srvset->absAcceptNewClient();

            if (!tmp)
                return ;
            this->cl_list->push_back(new UserManager<SCK>(tmp));
            if (this->newConnection)
                for (auto it = this->controllers.begin(); it != this->controllers.end(); ++it)
                    ((*it)->*(this->newConnection))(this->cl_list->back());
            this->network_monitor->addFd(dynamic_cast<IServerSocket<SCK>*>(this->cl_list->back()->getServerSocket()),
                                         static_cast<Enum::Flag>(Enum::READ | Enum::CLOSE));
            return ;
        }
        for (auto cli = this->cl_list->begin(); cli != this->cl_list->end(); ++cli) {
            for (int f = 0; (1 << f) <= Enum::CLOSE; ++f) {
                if (this->network_monitor->isObserved((*cli)->getServerSocket(),
                                                      static_cast<Enum::Flag>(1 << f))) {
                    if (!(this->*(this->monitor_action)[f])(*cli))
                        return (this->deleteClient(cli));
                }
            }
        }
        if (this->aobs)
            this->afterObserveAction();
    }

    virtual bool    readAction(UserManager<SCK> *cli) {
        int         n;

        cli->readFromMe();
        if (cli->emptyData())
            return (false);
        if (!cli->IsFilled())
            return (true);
		std::cout << "la taille des controllers vaut: " << this->controllers.size() << std::endl;
        for (auto it = this->controllers.begin(); it != this->controllers.end(); ++it) {
            if ((n = ((*it)->*(this->newData))(cli)) == 1)
            {
                cli->clearData();
                this->network_monitor->setObserver(cli->getServerSocket(),
                                                   static_cast<Enum::Flag>(Enum::WRITE |
                                                                           Enum::READ |
                                                                           Enum::CLOSE));
                return (true);
            }
            else if (n == 0)
                return (false);
        }
        cli->clearData();
        return (true);
    }

    virtual bool          writeAction(UserManager<SCK> *cli) {
        if (cli->sendStructEmpty()) {
            this->network_monitor->setObserver(cli->getServerSocket(),
                                               static_cast<Enum::Flag>(Enum::READ | Enum::CLOSE));
            return true;
        }
        if (!cli->writeOnMe())
            return (false);
        if (cli->sendStructEmpty()) {
            this->network_monitor->setObserver(cli->getServerSocket(),
                                               static_cast<Enum::Flag>(Enum::READ | Enum::CLOSE));
        }
        return (true);
    }

};

#endif
