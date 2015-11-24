#ifndef AUDIOCALLSYSTEM_HH_
# define AUDIOCALLSYSTEM_HH_

#include <list>
#include <SFML/Audio.hpp>
#include "UdpSocket.hh"
#include "Audio.hh"
#include "Recorder.hh"

class AudioCallSystem
{
public:
  AudioCallSystem();
  ~AudioCallSystem();
  bool in(UdpPacket &packet);
  UdpPacket *out();

private:
  void start();
  void addPacket(sf::SoundBuffer *buffer);
  Audio	_audio;
  std::list <UdpPacket *> _packets;
};

#endif //!AUDIOCALLSYSTEM_HH_
