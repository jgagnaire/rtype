#ifndef IUDPSOCKET_HH_
# define IUDPSOCKET_HH_

# include <cstddef>
# include <string>

class IUdpSocket
{
    public:
        IUdpSocket() {}
        virtual ~IUdpSocket() {}

        virtual bool            bind(unsigned short int port) = 0;
        virtual void            unbind() = 0;
        virtual uint64_t     send(const void *buf, uint64_t size,
                const std::string &ip, unsigned short int port) = 0;
        virtual uint64_t receive(void *buf, uint64_t size,
                std::string &ip, unsigned short &port) = 0;
};

#endif /* ifndef IUDPSOCKET_HH_ */
