#ifndef AUDIOCALLSYSTEM_HH_
# define AUDIOCALLSYSTEM_HH_

#include <list>
#include <thread>
#include <SFML/Audio.hpp>
#include "Network/UdpSocket.hh"
#include "Audio.hh"
#include "Recorder.hh"

class AudioCallSystem
{
public:
  AudioCallSystem();
  ~AudioCallSystem();
  bool in(UdpPacket *packet);
  UdpPacket *out();

private:
  void start();
  static void startThread(AudioCallSystem *obj);
  void addPacket(sf::SoundBuffer *buffer);
  Audio	_audio;
  bool _exit;
  std::list <UdpPacket *> _packets;
  std::thread	*_thread;
};

#endif //!AUDIOCALLSYSTEM_HH_
