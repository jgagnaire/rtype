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

struct UDPDataHeader {
    uint16_t packet_size;
    uint16_t query;
    uint64_t id;
}
# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  pragma pack(push, 1)
;
#  pragma pack(pop)
# else
__attribute__((__packed__));
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
    void		getMsg(const SCK s, std::string * const p) {
      size_t	ret;
      char	    *str = NULL;

      if (_get_pack.header.packet_size == 0) {
        _get_pack.data = "";
        return ;
      }
      if (!(ret = s->absReadFromClient(str, _get_pack.header.packet_size - _str_ret, p)))
        clearAll();
      else {
        _get_pack.data.insert(_get_pack.data.size(), str, ret);
        _str_ret += ret;
      }
    }
public:
      Packet() :  _wr_buff(NULL) { clearAll(); }

      virtual ~Packet() {};

      bool     packetEmpty() const { return (_rd_buff.empty()); }

      template <typename SCK>
      void		getPacket(const SCK s, std::string * const p = NULL) {
        size_t	ret;
        char    *str = NULL;

        if (_comdata_ret == sizeof(T))
          return getMsg(s, p);
        if (!(ret = s->absReadFromClient(str, sizeof(T) - _comdata_ret, p)))
          clearAll();
        if (ret) {
          _rd_buff.insert(_comdata_ret, str, ret);
          if (_comdata_ret == 0)
            _get_pack.data.clear();
          _comdata_ret += ret;
          if (_comdata_ret == sizeof(T))
  	        deserialize(_rd_buff.c_str());
        }
        delete [] str;
      }

      const PacketStruct    retrievePacket() const { return _get_pack; }

      void                  serialize() {
        char	*str = const_cast<char *>(_send_pack.front().data.c_str());
        char	*com = reinterpret_cast<char *>(&_send_pack.front().header);

        _wr_buff = new char [sizeof(T) + _send_pack.front().data.size()];
        std::copy(com, com + sizeof(T), _wr_buff);
        for (size_t i = 0;  i < sizeof(T); ++i)
          _wr_buff[i] = com[i];
        for (size_t i = 0;  i < _send_pack.front().data.size(); ++i)
          _wr_buff[sizeof(T) + i] = str[i];
      }

      void                  deserialize(const char *s, std::string str = "") {
        std::copy(s, s + sizeof(T), reinterpret_cast<char *>(&_get_pack));
        _get_pack.data = str;
      }

      void                  stockOnBuff(const T &c) {
        PacketStruct	ps;

        ps.header = c;
        _send_pack.push(ps);
      }

      void                  stockOnBuff(const std::string &s) { _send_pack.back().data = s; }
      bool                  isFilled() const { return _str_ret == _get_pack.header.packet_size; }
      bool                  HasSendPacket() const { return (!_send_pack.empty()); }
      bool                  sendPackEmpty() const { return (_send_pack.empty()); }


      void                  clearAll() {
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
