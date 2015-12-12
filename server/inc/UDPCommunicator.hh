#ifndef RTYPE_UDPCOMMUNICATOR_H
# define RTYPE_UDPCOMMUNICATOR_H

# include "IServerSocket.hh"
# include "ACommunicator.hh"
# include "Packet.hh"
# include "Enum.hh"
# include "GameManager.hh"

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

#  include "WinUDPSocketSet.hh"
#  include "WinServerMonitor.hh"
#  define init_memory(x, y) ZeroMemory((x), (y));
# else
#  include <strings.h>
#  include "UnixUDPSocketSet.hh"
#  include "UnixServerMonitor.hh"
#  define init_memory(x, y) bzero((x), (y));
# endif

template <typename USER, typename CONTROLLER,
        typename MONITOR, typename SCK>
class UDPCommunicator : public ACommunicator<USER, CONTROLLER, MONITOR, SCK> {
private:
    struct                  UDPData {
        UDPDataHeader       packet;
        char                buff[Enum::MAX_BUFFER_LENGTH];
    };

    UDPData                 _data;
    Packet<UDPDataHeader>   _packet;
    Packet<UDPDataHeader>::PacketStruct _pack;
public:
    UDPCommunicator(char *port) {
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        this->srvset = new WinUDPSocketSet(!port ? 1725 : std::atoi(port));
        this->network_monitor = new WinServerMonitor();
#else
        this->srvset = new UnixUDPSocketSet(!port ? 1726 : std::atoi(port));
        this->network_monitor = new UnixServerMonitor();
#endif
        this->network_monitor->addFd(dynamic_cast<IServerSocket<SCK>*>(this->srvset),
                                     Enum::READ);
        ::init_memory(&this->_data.buff[0], Enum::MAX_BUFFER_LENGTH);
    }

    virtual ~UDPCommunicator() {}

    virtual void launch(std::list<USER*> *cl) {
        GameManager<SCK>::instance().setUdpSocket(dynamic_cast<IServerSocket<SCK>*>(this->srvset));
        this->cl_list = cl;
        for (auto it = this->controllers.begin(); it != this->controllers.end(); ++it)
            (*it)->addUDPSocket(dynamic_cast<IServerSocket<SCK>*>(this->srvset));
        for (;;) {
            const char  *buff;
            std::string ip;

            this->network_monitor->observerFds();
            if (this->network_monitor->isObserved(dynamic_cast<IServerSocket<SCK> *>(this->srvset),
                                                  Enum::READ)) {
                size_t ret = _packet.getPacket(dynamic_cast<IServerSocket<SCK>*>(this->srvset),
                                               &ip, true);
                buff = _packet.getBuffer();
                // std::cout << ret << " " << sizeof(UDPData) << std::endl;
                std::copy(buff, buff + ret, reinterpret_cast<char *>(&_data));

                _pack.header = _data.packet;
                _pack.data = _data.buff;
                this->readAction(this->findUserByIP(ip));
                _packet.clearAll();
            }
        }
    }

    virtual bool	readAction(USER *cli) {
        if (!cli)
            return (true);
        cli->setUdpPacketStruct(_pack);
        for (auto it = this->controllers.begin(); it != this->controllers.end(); ++it) {
            if (((*it)->*(this->newData))(cli) == 1)
                return true;
        }
        return (true);
    }

    USER            *findUserByIP(const std::string &ip) {
        for (auto it = this->cl_list->begin(); it != this->cl_list->end(); ++it) {
            if ((*it)->getIP() == ip)
                return (*it);
        }
        return (0);
    }
};

#endif
