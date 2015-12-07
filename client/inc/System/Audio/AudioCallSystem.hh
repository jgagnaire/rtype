#ifndef AUDIOCALLSYSTEM_HH_
# define AUDIOCALLSYSTEM_HH_

#include <vector>
#include <thread>
#include <mutex>
#include <SFML/Audio.hpp>
#include "Network/UdpSocket.hh"
#include "Entity/Entity.hh"
#include "Recorder.hh"

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
  static void startThreadRecord(AudioCallSystem *obj);
  std::thread	*_recorderThread;

  void startPlay();
  static void startThreadPlay(AudioCallSystem *obj);
  void addBuffer(sf::SoundBuffer *buffer, const std::string &name);

  Recorder			*recorder;
  std::vector <Entity *>	_users;
  std::list <UdpPacket *>	_packets;
  std::thread			*_playerThread;
  std::mutex			_mutex;
  sf::Clock			_clock;
  sf::SoundBuffer		*toDelete;
  bool				_exit;
};

#endif //!AUDIOCALLSYSTEM_HH_
