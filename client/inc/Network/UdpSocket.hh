#ifndef UDPSOCKET_HH_
# define UDPSOCKET_HH_

# include <SFML/Network.hpp>
# include "IUdpSocket.hh"
# include "IPacket.hh"

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  pragma pack(1)
struct UdpHeader : public Header
{
    uint16_t    size;
    uint16_t    query;
    uint64_t    id;
};
#  pragma pack()
# else
struct UdpHeader : public Header
{
	uint16_t    size;
	uint16_t    query;
	uint64_t    id;
}__attribute__((__packed__));
# endif

class UdpPacket : public IPacket
{
    public:
        UdpPacket() {
            _header.size = 0;
            _header.query = 0;
            _header.id = 0;
            _data = 0;
        }
        virtual ~UdpPacket() {}

		virtual uint16_t    getSize() const { return this->_header.size; }
		virtual uint16_t    getQuery() const { return this->_header.query; }
		virtual const void  *getData() const { return this->_data; }

		virtual void        setSize(uint16_t size) { this->_header.size = size; }
		virtual void        setQuery(uint16_t query) { this->_header.query = query; }
		virtual void        setData(const void *data) { this->_data = data; }

		virtual Header           &getHeader() { return this->_header; }
		virtual const Header     &getHeader() const { return this->_header; }
		virtual uint64_t        getID() const { return this->_header.id; }
		virtual void            setID(uint64_t id) { this->_header.id = id; }

    private:
        UdpHeader   _header;
};

class UdpSocket : public IUdpSocket
{
    public:
		UdpSocket() {}
		virtual ~UdpSocket() {}

        virtual bool        bind(unsigned short int port);
		virtual void	unbind() { this->_socket.unbind(); }
        virtual uint64_t send(const void *buf, uint64_t size,
			const std::string &ip, unsigned short int port) { return (this->_socket.send(buf, size, ip, port) == sf::Socket::Done); }
        virtual uint64_t receive(void *buf, uint64_t size,
                std::string &ip, unsigned short &port);

    private:
        sf::UdpSocket       _socket;
};

#endif /* ifndef UDPSOCKET_HH_ */
