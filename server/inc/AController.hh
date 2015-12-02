#pragma once
# include <list>
# include <time.h>
# include "Enum.hh"
# include "UserManager.hh"
# include "IServerMonitor.hh"

template<typename T>
class AController {
public:
    AController(std::list<UserManager<T> *> &cli)
            : cl_list(cli) {}

    virtual ~AController() {}
    virtual int	newConnection(UserManager<T> *) const { return (-1); }
    virtual int	closeConnection(UserManager<T> *) const { return (-1); }
    virtual int	newData(UserManager<T> *cli) = 0;

    void		timeout(std::list<UserManager<T> *> &cl_list,
                        IServerMonitor<T> *net_monitor) const {
        static      time_t t = 0;

        if (!t) {
            t = time(0);
            return ;
        }
        if (time(0) - t >= Enum::MAX_TIME) {
            for (auto it = cl_list.begin(); it != cl_list.end();) {
                if ((*it)->getPing()) {
                    (*it)->setPing(false);
                    (*it)->sendPing();
                    net_monitor->setObserver((*it)->getServerSocket(),
                                             static_cast<Enum::Flag>(Enum::WRITE |
                                                                     Enum::READ |
                                                                     Enum::CLOSE));
                    ++it;
                }
                else {
                    net_monitor->deleteFd((*it)->getServerSocket());
                    (*it)->disconnect();
                    delete *it;
                    *it = 0;
                    cl_list.erase(it++);
                }
            }
            t = time(0);
        }
    }

    UserManager<T>	*findUserByName(const std::string &name) const {
        if (name.empty())
            return (0);
        for (auto it = cl_list.begin(); it != cl_list.end(); ++it) {
            if (name == (*it)->getName())
                return (*it);
        }
        return (0);
    }

protected:
    std::list<UserManager<T> *>	&cl_list;
};
