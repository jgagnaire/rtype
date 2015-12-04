#ifndef AUDIOCALLSYSTEM_HH_
# define AUDIOCALLSYSTEM_HH_

#include <vector>
#include <thread>
#include <mutex>
#include <SFML/Audio.hpp>
#include "Network/UdpSocket.hh"
#include "Entity/Entity.hh"
#include "Recorder.hh"
#include "AudioBuffer.hh"

class Recorder;

class AudioCallSystem
{
public:
  AudioCallSystem();
  ~AudioCallSystem();
  void addPacket(sf::SoundBuffer *buffer);
  bool in(UdpPacket *packet);
  UdpPacket *out();

private:
  void startRecord();
  void startPlay();
  static void startThreadRecord(AudioCallSystem *obj);
  static void startThreadPlay(AudioCallSystem *obj);
  void addBuffer(sf::SoundBuffer *buffer, const std::string &name);
  std::vector <Entity *> _users;
  bool _exit;
  std::list <UdpPacket *> _packets;
  std::thread	*_recorderThread;
  std::thread	*_playerThread;
std::mutex	_mutex;
Recorder	*recorder;
};

#endif //!AUDIOCALLSYSTEM_HH_
