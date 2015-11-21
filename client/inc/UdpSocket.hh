#ifndef UDPSOCKET_HH_
# define UDPSOCKET_HH_

# include <cstdint>
# include <SFML/Network.hpp>
# include "IUdpSocket.hh"

typedef struct s_UdpHeader
{
    uint16_t    size;
    uint16_t    query;
    uint64_t    id;
} UdpHeader;

class UdpPacket
{
    public:
        UdpHeader   &getHeader();

        uint16_t    getSize() const;
        uint16_t    getQuery() const;
        uint64_t    getID() const;
        void        *getData() const;

        void        setSize(uint16_t size);
        void        setQuery(uint16_t query);
        void        setID(uint64_t id);
        void        setData(void *data);

    private:
        UdpHeader   _header;
        void        *_data;
};

class UdpSocket : IUdpSocket
{
    public:
        UdpSocket();
        virtual ~UdpSocket();

        virtual bool        bind(unsigned short int port);
        virtual void        unbind();
        virtual std::size_t send(const void *buf, std::size_t size,
                const std::string &ip, unsigned short int port);
        virtual std::size_t receive(void *buf, std::size_t size,
                std::string &ip, unsigned short &port);

    private:
        sf::UdpSocket       _socket;
};

#endif /* ifndef UDPSOCKET_HH_ */
