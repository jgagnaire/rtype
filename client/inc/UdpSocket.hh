#ifndef UDPSOCKET_HH_
# define UDPSOCKET_HH_

# include <SFML/Network.hpp>
# include "IUdpSocket.hh"

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
