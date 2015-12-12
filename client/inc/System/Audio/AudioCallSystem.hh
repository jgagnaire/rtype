#ifndef AUDIOCALLSYSTEM_HH_
# define AUDIOCALLSYSTEM_HH_

#include <vector>
#include <thread>
#include <mutex>
#include <SFML/Audio.hpp>
#include "System/ASystem.hh"
#include "Network/UdpSocket.hh"
#include "Entity/Entity.hh"
#include "Recorder.hh"

class Recorder;

class AudioCallSystem : public ASystem
{
public:
  AudioCallSystem();
  virtual ~AudioCallSystem();

  void addPacket(sf::SoundBuffer *buffer);

  virtual void update(IClock &) {}
  virtual bool handle(EventSum) { return true; }
  virtual std::vector<REvent> broadcast(void) { return _eventList; }
  virtual EventSum getEvent() { return 0; }
  virtual void in(IPacket *packet);
  virtual IPacket *out();

private:
  void startPlay();
  static void startThread(AudioCallSystem *obj);
  void addBuffer(sf::SoundBuffer *buffer, const std::string &name);

  Recorder			*recorder;
  std::vector <Entity *>	_users;
  std::list <IPacket *>		_packets;
  std::thread			*_thread;
  std::mutex			_mutex;
  sf::Clock			_clock;
  bool				_exit;
};

#endif //!AUDIOCALLSYSTEM_HH_
