#include "AudioCallSystem.hh"

AudioCallSystem::AudioCallSystem()
{
  this->start();
}

AudioCallSystem::~AudioCallSystem()
{
}

void AudioCallSystem::start()
{
  sf::SoundBuffer *buffer;
  Recorder recorder;

  _audio.startAudio();
  recorder.start();
  while (sf::SoundBufferRecorder::isAvailable())
    {
      if ((buffer = recorder.getBuffer()))
	{
	  this->addPacket(buffer);
	}
    }
}

void AudioCallSystem::addPacket(sf::SoundBuffer *buffer)
{

}

bool AudioCallSystem::in(UdpPacket &packet)
{
	static_cast<void>(packet);
	return true; //Temporary
}

UdpPacket *AudioCallSystem::out()
{
  UdpPacket *tmp;

  if (_packets.empty())
    return (0);
  tmp = _packets.front();
  _packets.pop_front();
  return (tmp);
}
