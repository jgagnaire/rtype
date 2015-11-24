#include "AudioCall.hh"

AudioCall::AudioCall()
{
  this->start();
}

AudioCall::~AudioCall()
{
}

void AudioCall::start()
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

void AudioCall::addPacket(sf::SoundBuffer *buffer)
{

}

bool AudioCall::in(UdpPacket &packet)
{
}

UdpPacket *AudioCall::out()
{
  UdpPacket *tmp;

  if (_packets.empty())
    return (0);
  tmp = _packets.front();
  _packets.pop_front();
  return (tmp);
}
