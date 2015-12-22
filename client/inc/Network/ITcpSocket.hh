#ifndef ITCPSOCKET_HH_
# define ITCPSOCKET_HH_

# include <cstddef>
# include <string>

class ITcpSocket
{
    public:
        ITcpSocket() {}
        virtual ~ITcpSocket() {}

        virtual bool            connect(const std::string &ip,
                unsigned short int port) = 0;
        virtual uint64_t     send(const void *buf, uint64_t size) = 0;
        virtual uint64_t     receive(void *buf, uint64_t size) = 0;
};

#endif /* ifndef ITCPSOCKET_HH_ */
