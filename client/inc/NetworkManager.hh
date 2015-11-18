#ifndef NETWORKMANAGER_HH_
# define NETWORKMANAGER_HH_

# include <list>
# include <cstdint>
# include "UdpSocket.hh"
# include "TcpSocket.hh"

namespace NetworkState
{
    char tcp = 1;
    char udp = 2;
}

class Packet
{
    public:
        Packet();
        virtual ~Packet();

        void            setPacketSize(uint16_t packetSize);
        void            setQuery(uint16_t query);
        void            setData(void *data, std::size_t size);

        uint16_t        getPacketSize() const;
        uint16_t        getQuery() const;
        void            *getData() const;
        std::size_t     getDataSize() const;

    private:
        uint16_t        _packetSize;
        uint16_t        _query;
        void            *_data;
        std::size_t     _dataSize;
};

class NetworkManager
{
    public:
        NetworkManager();
        virtual ~NetworkManager();

        void    send(const Packet &packet);
        void    receive();
        Packet  getPacket();

    private:
        std::list<Packet>             _packetList;
        UdpSocket                     _udp;
        TcpSocket                     _tcp;
        char                          _state;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
