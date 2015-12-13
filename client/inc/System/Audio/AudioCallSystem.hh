#ifndef AUDIOCALLSYSTEM_HH_
# define AUDIOCALLSYSTEM_HH_

#include <vector>
#include <mutex>
#include "Recorder.hh"
#include "System/ASystem.hh"
#include "Network/UdpSocket.hh"
#include "Entity/Entity.hh"
#include "Utility/ThreadFactory.hh"
#include "Utility/IThread.hh"
#include "Utility/Sound.hh"
#include "Utility/RTime.hh"

class Recorder;

class AudioCallSystem : public ASystem
{
public:
  AudioCallSystem();
  virtual ~AudioCallSystem();

  void addPacket(SoundBuffer *buffer);

  virtual void update(int) {}
  inline virtual bool handle(EventSum e)
  {
    if (e & Key_Sound)
      this->recorder->changeState();
    return true;
  }
  inline virtual std::vector<REvent> &broadcast(void)
  {
    return _eventList;
  }
  inline virtual EventSum getEvent()
  {
    return noEvent;
  }
  virtual void in(IPacket *packet);
  virtual IPacket *out();

private:
  static const uint16_t CODE_SEND_PACKET = 302;
  static const uint16_t CODE_RECEIVE_PACKET = 404;

  void startPlay();
  static void startThread(AudioCallSystem *obj);
  void addBuffer(ISoundBuffer *buffer, const std::string &name);
  std::string getPseudo(const void *data, uint16_t packetSize) const;

  Recorder			*recorder;
  std::vector <Entity *>	_users;
  std::list <IPacket *>		_packets;
  IThread <void, AudioCallSystem *> *_thread;
  std::mutex				_mutex;
  bool				_exit;
};

#endif //!AUDIOCALLSYSTEM_HH_
