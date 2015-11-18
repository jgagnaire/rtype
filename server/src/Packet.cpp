#include "Packet.h"

Packet::Packet() :  _wr_buff(NULL) {
  clearAll();
}

Packet::~Packet() {}

bool	Packet::isFilled() const {
  return _str_ret == _get_pack.com.packet_size;
}

bool	Packet::HasSendPacket() const { return (!_send_pack.empty()); }

void	Packet::clearAll() {
  _rd_buff.clear();
  _comdata_ret = 0;
  _str_ret = 0;
}

const PacketStruct	&Packet::retrievePacket() const {
  return _get_pack;
}

void	Packet::serialize() {
  char	*str = const_cast<char *>(_send_pack.front().data.c_str());
  char	*com = reinterpret_cast<char *>(&_send_pack.front().com);

  _wr_buff = new char [sizeof(ComData) + _send_pack.front().data.size()];
  std::copy(com, com + sizeof(ComData), _wr_buff);
  for (size_t i = 0;  i < sizeof(ComData); ++i)
    _wr_buff[i] = com[i];
  for (size_t i = 0;  i < _send_pack.front().data.size(); ++i)
    _wr_buff[sizeof(ComData) + i] = str[i];
}

void	Packet::deserialize(const char *s, std::string str) {
  std::copy(s, s + sizeof(ComData), reinterpret_cast<char *>(&_get_pack));
  _get_pack.data = str;
}

bool	Packet::sendPackEmpty() const { return (_send_pack.empty()); }

void	Packet::stockOnBuff(const std::string &s) {
  _send_pack.back().data = s;
}

void	Packet::stockOnBuff(const ComData &c) {
  PacketStruct	ps;

  ps.com = c;
  _send_pack.push(ps);
}

bool	Packet::packetEmpty() const { return (_rd_buff.empty()); }
