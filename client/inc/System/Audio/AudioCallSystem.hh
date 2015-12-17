#ifndef AUDIOCALLSYSTEM_HH_
# define AUDIOCALLSYSTEM_HH_

#include <vector>
#include "System/ASystem.hh"
#include "Network/UdpSocket.hh"
#include "Entity/Entity.hh"
#include "Utility/Sound.hh"
#include "Utility/RTime.hh"
#include "Recorder.hh"

class AudioCallSystem : public ASystem
{
public:
  AudioCallSystem();
  virtual ~AudioCallSystem();

  void addPacket(SoundBuffer *buffer);

  virtual void update(int);
  virtual bool handle(EventSum e)
  {
    if (e & Key_Sound)
      this->_recorder.changeState();
    return true;
  }
  virtual std::vector<REvent> &broadcast(void)
  {
    return _eventList;
  }
  virtual EventSum getEvent()
  {
    return noEvent;
  }
  virtual void in(IPacket *packet);
  virtual IPacket *out();

private:
  static const uint16_t CODE_SEND_PACKET = 302;
  static const uint16_t CODE_RECEIVE_PACKET = 404;

  void addBuffer(ISoundBuffer *buffer, const std::string &name);
  std::string getPseudo(const void *data, uint16_t packetSize) const;

  Recorder			_recorder;
  std::vector <Entity *>	_users;
  std::list <IPacket *>		_packets;
};

#endif //!AUDIOCALLSYSTEM_HH_
