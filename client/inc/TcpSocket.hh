#ifndef TCPSOCKET_HH_
# define TCPSOCKET_HH_

# include <SFML/Network.hpp>
# include "ITcpSocket.hh"
# include "IPacket.hh"

typedef struct s_TcpHeader : public Header
{
} TcpHeader;

class TcpPacket : public IPacket
{
    public:
        TcpPacket() {}
        virtual ~TcpPacket() {}

        virtual uint16_t    getSize() const;
        virtual uint16_t    getQuery() const;
        virtual void        *getData() const;

        virtual void        setSize(uint16_t size);
        virtual void        setQuery(uint16_t query);
        virtual void        setData(void *data);

        virtual Header           &getHeader();
        virtual const Header     &getHeader() const;

    private:
        TcpHeader   _header;
};

class TcpSocket : public ITcpSocket
{
    public:
        TcpSocket();
        ~TcpSocket();

        virtual bool            connect(const std::string &ip,
                unsigned short int port);
        virtual std::size_t     send(const void *buf, std::size_t size);
        virtual std::size_t     receive(void *buf, std::size_t size);
    private:
        sf::TcpSocket           _socket;
};

#endif /* ifndef TCPSOCKET_HH_ */
