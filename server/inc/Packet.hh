#pragma once
# include <cstdint>
# include <queue>
# include <iostream>
# include <algorithm>
# include "IServerSocket.hh"

struct ComData {
	uint16_t	packet_size;
	uint16_t	query;
}
# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  pragma pack(push, 1)
;
#  pragma pack(pop)
# else
__attribute__((__packed__));
# endif

struct PacketStruct {
  ComData	com;
  std::string	data;
};

class Packet {
public:
  Packet();
  virtual ~Packet();

  bool  packetEmpty() const;

  template <typename SCK>
  void		getPacket(const SCK s) {
    size_t	ret;
    char       	*str = NULL;

    if (_comdata_ret == sizeof(ComData))
      return getMsg(s);
    if (!(ret = s->absReadFromClient(str, sizeof(ComData) - _comdata_ret)))
      clearAll();
    if (ret) {
      _rd_buff.insert(_comdata_ret, str, ret);
      if (_comdata_ret == 0)
	_get_pack.data.clear();
      _comdata_ret += ret;
      if (_comdata_ret == sizeof(ComData))
	deserialize(_rd_buff.c_str());
    }
    delete [] str;
  }

  void	serialize();
  void	deserialize(const char *s, std::string str = "");
  void	stockOnBuff(const ComData &);
  void	stockOnBuff(const std::string &);
  bool		isFilled() const;
  const PacketStruct	&retrievePacket() const;
  bool		HasSendPacket() const;
  void		clearAll();
  bool		sendPackEmpty() const;
  template <typename SCK>
  bool    sendPacket(const SCK s) {
    bool	ret;

    ret = s->absWriteOnClient(_wr_buff,
			      sizeof(ComData) + _send_pack.front().data.size());
    delete  [] _wr_buff;
    _wr_buff = NULL;
    if (!_send_pack.empty())
      _send_pack.pop();
    return (ret);
  }
private:
  template <typename SCK>
  void		getMsg(const SCK s) {
    size_t	ret;
    char	*str = NULL;

    if (_get_pack.com.packet_size == 0) {
      _get_pack.data = "";
      return ;
    }
    if (!(ret = s->absReadFromClient(str, _get_pack.com.packet_size - _str_ret)))
      clearAll();
    else {
      _get_pack.data.insert(_get_pack.data.size(), str, ret);
      _str_ret += ret;
    }
  }
  PacketStruct			_get_pack;
  std::queue<PacketStruct>	_send_pack;
  char				*_wr_buff;
  std::string			_rd_buff;
  size_t			_comdata_ret;
  size_t			_str_ret;
};
