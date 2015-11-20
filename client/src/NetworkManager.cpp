#include "NetworkManager.hh"

Packet::Packet():
    _packetSize(0), _query(0), _data(0), _dataSize(0)
{}

Packet::~Packet()
{
}

void    Packet::setPacketSize(uint16_t packetSize)
{
    _packetSize = packetSize;
}

void    Packet::setQuery(uint16_t query)
{
    _query = query;
}

void    Packet::setData(void *data, std::size_t size)
{
    _data = data;
    _dataSize = size;
}

uint16_t        Packet::getPacketSize() const
{
     return _packetSize;
}

uint16_t        Packet::getQuery() const
{
    return _query;
}

void            *Packet::getData() const
{
     return _data;
}

std::size_t     Packet::getDataSize() const
{
    return _dataSize;
}

NetworkManager::NetworkManager()
{}

NetworkManager::~NetworkManager()
{}

void    NetworkManager::send(const Packet &packet)
{
    if (_state == NetworkState::tcp)
    {
        //_tcp.send();
    }
    else
    {

    }
}
