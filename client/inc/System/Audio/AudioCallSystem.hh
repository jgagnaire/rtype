#ifndef AUDIOCALLSYSTEM_HH_
# define AUDIOCALLSYSTEM_HH_

#include <vector>
#include <thread>
#include <mutex>
#include <SFML/Audio.hpp>
#include "Recorder.hh"
#include "System/ASystem.hh"
#include "Network/UdpSocket.hh"
#include "Entity/Entity.hh"
#include "Utility/ThreadFactory.hh"
#include "Utility/IThread.hh"
#include "Utility/IMutex.hh"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
# include "Utility/WinMutex.hh"
#else
# include "Utility/UnixMutex.hh"
#endif

class Recorder;

class AudioCallSystem : public ASystem
{
public:
  AudioCallSystem();
  virtual ~AudioCallSystem();

  void addPacket(sf::SoundBuffer *buffer);

  virtual void update(IClock &) {}
  inline virtual bool handle(EventSum) { return true; }
  inline virtual std::vector<REvent> &broadcast(void) { return _eventList; }
  inline virtual EventSum getEvent() { return 0; }
  virtual void in(IPacket *packet);
  virtual IPacket *out();

private:
  static const uint16_t CODE_SEND_PACKET = 302;
  static const uint16_t CODE_RECEIVE_PACKET = 404;

  void startPlay();
  static void startThread(AudioCallSystem *obj);
  void addBuffer(sf::SoundBuffer *buffer, const std::string &name);
  std::string getPseudo(const void *data, uint16_t packetSize) const;

  Recorder			*recorder;
  std::vector <Entity *>	_users;
  std::list <IPacket *>		_packets;
  IThread <void, AudioCallSystem *> *_thread;
  IMutex			*_mutex;
  bool				_exit;
};

#endif //!AUDIOCALLSYSTEM_HH_
