#ifndef UDPSOCKET_HH_
# define UDPSOCKET_HH_

# include <SFML/Network.hpp>
# include "IUdpSocket.hh"
# include "IPacket.hh"

typedef struct s_UdpHeader : public Header
{
    uint64_t    id;
} __attribute__((packed)) UdpHeader;

class UdpPacket : public IPacket
{
    public:
        UdpPacket() {}
        virtual ~UdpPacket() {}

        virtual uint16_t    getSize() const;
        virtual uint16_t    getQuery() const;
        virtual void        *getData() const;

        virtual void        setSize(uint16_t size);
        virtual void        setQuery(uint16_t query);
        virtual void        setData(void *data);

        virtual Header           &getHeader();
        virtual const Header     &getHeader() const;
        virtual uint64_t        getID() const;
        virtual void            setID(uint64_t id);

    private:
        UdpHeader   _header;
};

class UdpSocket : public IUdpSocket
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
