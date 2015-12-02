#include <algorithm>
#include "AudioCallSystem.hh"

AudioCallSystem::AudioCallSystem()
{
  this->_exit = false;
  this->_thread = 0;
  this->start();
}

AudioCallSystem::~AudioCallSystem()
{
  this->_exit = true;
  this->_thread->join();
  this->_packets.clear();
  delete this->_thread;
}

void	AudioCallSystem::startThread(AudioCallSystem *obj)
{
  obj->start();  
}

void AudioCallSystem::start()
{
  sf::SoundBuffer *buffer;
  Recorder recorder;

  if (!_thread)
    {
      _thread = new std::thread(&AudioCallSystem::startThread, this);
      return ;
    }
  _audio.startAudio();
  recorder.start();
  while (sf::SoundBufferRecorder::isAvailable() && !_exit)
    {
      if ((buffer = recorder.getBuffer()))
	{
	  this->addPacket(buffer);
	  in(out());
	}
    }
  recorder.stop();
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
