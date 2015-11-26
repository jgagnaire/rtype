#include <algorithm>
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
  std::string pseudo = "lol";
  UdpPacket *tmp = new UdpPacket();
  short int *data;
  const short int *tmpData;

  tmp->setSize(buffer->getSampleCount() * 2);
  data = new short int[tmp->getSize()];
  tmp->setQuery(502);
  tmpData = buffer->getSamples();
  std::copy(tmpData, tmpData + buffer->getSampleCount() * 2, data);
  tmp->setData(data);
  _packets.push_back(tmp);
  delete buffer;
}

bool AudioCallSystem::in(UdpPacket *packet)
{
  sf::SoundBuffer *buffer = new sf::SoundBuffer();
  short int *data;
  short int *tmpData;

  if (!packet)
    return false;
  tmpData = (short int *)(packet->getData());
  data = new short int[packet->getSize()];
  std::copy(tmpData, tmpData + packet->getSize(), data);
  buffer->loadFromSamples(data, packet->getSize() / 2, 2, packet->getSize() / 2);
  this->_audio.addBuffer(buffer, "toto");
  return true;
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
