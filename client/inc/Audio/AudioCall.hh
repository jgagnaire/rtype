#ifndef AUDIOCALL_HH_
# define AUDIOCALL_HH_

#include <list>
#include <SFML/Audio.hpp>
#include "UdpSocket.hh"
#include "Audio.hh"
#include "Recorder.hh"

class AudioCall
{
public:
  AudioCall();
  ~AudioCall();
  bool in(UdpPacket &packet);
  UdpPacket *out();

private:
  void start();
  void addPacket(sf::SoundBuffer *buffer);
  Audio	_audio;
  std::list <UdpPacket *> _packets;
};

#endif //!AUDIOCALL_HH_
