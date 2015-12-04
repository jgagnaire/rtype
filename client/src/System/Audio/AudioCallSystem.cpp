#include <algorithm>
#include "AudioCallSystem.hh"

AudioCallSystem::AudioCallSystem()
{
  this->_exit = false;
  this->_recorderThread = 0;
  this->_playerThread = 0;
  recorder = new Recorder(this);
  recorder->start();
  this->startPlay();
  this->startRecord();
}

AudioCallSystem::~AudioCallSystem()
{
  this->_exit = true;
  recorder->stop();
  delete recorder;
  this->_recorderThread->join();
  this->_playerThread->join();
  this->_packets.clear();

  delete this->_playerThread;
  delete this->_recorderThread;
}

void	AudioCallSystem::startThreadRecord(AudioCallSystem *obj)
{
  obj->startRecord();  
}

void	AudioCallSystem::startThreadPlay(AudioCallSystem *obj)
{
  obj->startPlay();  
}

void AudioCallSystem::startRecord()
{
  sf::SoundBuffer *buffer;
  Recorder recorder(this);

  if (!_recorderThread)
    {
      _recorderThread = new std::thread(&AudioCallSystem::startThreadRecord, this);
      return ;
    }
  while (!_exit)
    {
      if (!_packets.empty())
      in(out());
    }
}

void	AudioCallSystem::startPlay()
{
  std::vector <Entity *>::iterator it;
  sf::SoundBuffer *tmp = 0;

  if (!_playerThread)
    {
      _playerThread = new std::thread(&AudioCallSystem::startThreadPlay, this);
      return ;
    }
  while (!_exit)
    {
      _mutex.lock();
      if (!this->_users.empty())
	for (it = this->_users.begin(); it != this->_users.end(); ++it)
	  {
	    if (!((*it)->manager.getAll<sf::SoundBuffer *>().empty()))
	      {
		tmp = (*it)->manager.getAll<sf::SoundBuffer *>().front();
		break ;
	      }
	  }
      if (tmp && (*it)->manager.get<sf::Clock *>("clock")->getElapsedTime().asMicroseconds() >=
	  (*it)->manager.get<sf::Time *>("time")->asMicroseconds())
	{
	  
	}
      _mutex.unlock();
    }
}

void AudioCallSystem::addBuffer(sf::SoundBuffer *buffer, const std::string &name)
{
  Entity	*tmp;
  std::vector <Entity *>::iterator it;

  for (it = this->_users.begin(); it != this->_users.end(); ++it)
    {
      if ((*it)->manager.get<std::string>("name") == name)
	break ;
    }
  if (it != this->_users.end())
    (*it)->manager.add<sf::SoundBuffer *>("", buffer);
  else
    {
      tmp = new Entity;
      tmp->manager.add<std::string>("name", name);
      tmp->manager.add<sf::Clock *>("clock", new sf::Clock);
      tmp->manager.add<sf::Time *>("time", new sf::Time);
      tmp->manager.add<sf::SoundBuffer *>("", buffer);
      this->_users.push_back(tmp);
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
  _mutex.lock();
  this->addBuffer(buffer, "toto");
  _mutex.unlock();
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
