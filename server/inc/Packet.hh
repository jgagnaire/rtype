#pragma once
# include <cstdint>
# include <queue>
# include <iostream>
# include <algorithm>
# include "IServerSocket.hh"

struct TCPDataHeader {
    uint16_t packet_size;
    uint16_t query;
};

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  pragma pack(1)
struct UDPDataHeader {
    uint16_t packet_size;
    uint16_t query;
    uint64_t id;
};
#  pragma pack()
# else
struct UDPDataHeader {
	uint16_t packet_size;
	uint16_t query;
	uint64_t id;
}__attribute__((__packed__));
# endif

template <typename T>
class Packet {
public:
    struct    PacketStruct {
        T	        header;
        std::string data;
    };
private:
    PacketStruct			    _get_pack;
    std::queue<PacketStruct>	_send_pack;
    char				        *_wr_buff;
    std::string			        _rd_buff;
    size_t			            _comdata_ret;
    size_t			            _str_ret;

    template <typename SCK>
    size_t		getMsg(const SCK s, std::string * const p) {
      size_t	ret;
      char	    *str = 0;

      if (_get_pack.header.packet_size == 0) {
        _get_pack.data = "";
        return (0);
      }
      if (!(ret = s->absReadFromClient(str, _get_pack.header.packet_size - _str_ret, p)))
        clearAll();
      else {
        _get_pack.data.insert(_get_pack.data.size(), str, ret);
        _str_ret += ret;
      }
      return (ret);
    }
public:
      Packet() :  _wr_buff(0) { clearAll(); }

      virtual ~Packet() {};

      bool     packetEmpty() const { return (_rd_buff.empty()); }

      template <typename SCK>
      size_t		getPacket(const SCK s, std::string * const p = NULL, bool max_read = false) {
        size_t	ret;
        char    *str = NULL;
        std::size_t  max_val = sizeof(T) + 512; // TODO, magic number;

        if (!max_read)
          max_val = sizeof(T);
        if (_comdata_ret == max_val)
          return getMsg(s, p);
        if (!(ret = s->absReadFromClient(str, max_val - _comdata_ret, p)))
          clearAll();
        if (ret) {
          _rd_buff.insert(_comdata_ret, str, ret);
          if (_comdata_ret == 0)
            _get_pack.data.clear();
          _comdata_ret += ret;
          if (_comdata_ret == max_val)
  	        deserialize(_rd_buff.c_str());
        }
        delete [] str;
        return (ret);
      }

      const PacketStruct    retrievePacket() const { return _get_pack; }

      void                  serialize() {
        char	*str = const_cast<char *>(_send_pack.front().data.c_str());
        char	*com = reinterpret_cast<char *>(&_send_pack.front().header);

        _wr_buff = new char [sizeof(T) + _send_pack.front().data.size() + 1];
        std::copy(com, com + sizeof(T), _wr_buff);
        for (std::size_t i = 0;  i < sizeof(T); ++i)
          _wr_buff[i] = com[i];
        for (std::size_t i = 0;  i < _send_pack.front().data.size(); ++i)
          _wr_buff[sizeof(T) + i] = str[i];
      }

      inline
      void                  deserialize(const char *s, std::string str = "") {
        std::copy(s, s + sizeof(T), reinterpret_cast<char *>(&_get_pack));
        _get_pack.data = str;
        std::cout << "la query: " << _get_pack.header.query << std::endl;
      }

      inline
      void                  stockOnBuff(const T &c) {
        PacketStruct	ps;

        ps.header = c;
        _send_pack.push(ps);
      }

      inline void                  stockOnBuff(const std::string &s) { _send_pack.back().data = s; }
      inline bool                  isFilled() const { return _str_ret == _get_pack.header.packet_size; }
      inline bool                  HasSendPacket() const { return (!_send_pack.empty()); }
      inline bool                  sendPackEmpty() const { return (_send_pack.empty()); }
      inline const char            *getBuffer() const { return (_rd_buff.c_str()); }

      inline void                  clearAll() {
        _rd_buff.clear();
        _comdata_ret = 0;
        _str_ret = 0;
      }

      template <typename SCK>
      bool                  sendPacket(const SCK s, const std::string &ip = "", const std::string &port = "") {
        bool	                ret;


        ret = s->absWriteOnClient(_wr_buff, sizeof(T) + _send_pack.front().data.size(),
                                  ip, port);
        delete  [] _wr_buff;
        _wr_buff = NULL;
        if (!_send_pack.empty())
          _send_pack.pop();
        return (ret);
      }
};
