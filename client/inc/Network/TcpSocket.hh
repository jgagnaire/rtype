#ifndef TCPSOCKET_HH_
# define TCPSOCKET_HH_

# include <SFML/Network.hpp>
# include "ITcpSocket.hh"
# include "IPacket.hh"

struct TcpHeader : public Header
{
    uint16_t    size;
    uint16_t    query;
};

class TcpPacket : public IPacket
{
public:
    TcpPacket() {
        _header.size = 0;
        _header.query = 0;
        _data = 0;
    }
	virtual ~TcpPacket() {}

	virtual uint16_t    getSize() const { return this->_header.size; }
	virtual uint16_t    getQuery() const { return this->_header.query; }
	virtual const void  *getData() const { return this->_data; }

	virtual void        setSize(uint16_t size) { this->_header.size = size; }
	virtual void        setQuery(uint16_t query) { this->_header.query = query; }
	virtual void        setData(const void *data) { this->_data = data; }

	virtual Header           &getHeader() { return this->_header; }
	virtual const Header     &getHeader() const { return this->_header; }

    private:
        TcpHeader   _header;
};

class TcpSocket : public ITcpSocket
{
    public:
		TcpSocket() {}
		virtual ~TcpSocket() {}

        virtual bool            connect(const std::string &ip,
                unsigned short int port);
        virtual std::size_t     send(const void *buf, std::size_t size);
        virtual std::size_t     receive(void *buf, std::size_t size);
    private:
        sf::TcpSocket           _socket;
};

#endif /* ifndef TCPSOCKET_HH_ */
