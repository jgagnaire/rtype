#ifndef TCPSOCKET_HH_
# define TCPSOCKET_HH_

# include <SFML/Network.hpp>
# include "ITcpSocket.hh"

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
