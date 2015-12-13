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

		inline virtual uint16_t    getSize() const { return this->_header.size; }
		inline virtual uint16_t    getQuery() const { return this->_header.query; }
		inline virtual const void  *getData() const { return this->_data; }

		inline virtual void        setSize(uint16_t size) { this->_header.size = size; }
		inline virtual void        setQuery(uint16_t query) { this->_header.query = query; }
		inline virtual void        setData(const void *data) { this->_data = data; }

		inline virtual Header           &getHeader() { return this->_header; }
		inline virtual const Header     &getHeader() const { return this->_header; }
		inline virtual uint64_t        getID() const { return this->_header.id; }
		inline virtual void            setID(uint64_t id) { this->_header.id = id; }

    private:
        UdpHeader   _header;
};

class UdpSocket : public IUdpSocket
{
    public:
		UdpSocket() {}
		virtual ~UdpSocket() {}

        virtual bool        bind(unsigned short int port);
		inline virtual void	unbind() { this->_socket.unbind(); }
        inline virtual std::size_t send(const void *buf, std::size_t size,
			const std::string &ip, unsigned short int port) { return (this->_socket.send(buf, size, ip, port) == sf::Socket::Done); }
        virtual std::size_t receive(void *buf, std::size_t size,
                std::string &ip, unsigned short &port);

    private:
        sf::UdpSocket       _socket;
};

#endif /* ifndef UDPSOCKET_HH_ */
