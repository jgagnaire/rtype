#include <algorithm>
#include "AudioCallSystem.hh"

AudioCallSystem::AudioCallSystem():
	recorder(new Recorder(this)), _thread(0), _exit(false)
{
  recorder->start();
  this->startPlay();
}

AudioCallSystem::~AudioCallSystem()
{
  this->_exit = true;
  recorder->stop();
  delete recorder;
  this->_thread->join();
  this->_packets.clear();
  delete this->_thread;
}

void	AudioCallSystem::startThread(AudioCallSystem *obj)
{
  obj->startPlay();  
}

void	AudioCallSystem::startPlay()
{
  std::vector <Entity *>::iterator it;
  sf::SoundBuffer *tmp;
  sf::Sound *sound;

  if (!_thread)
    {
      _thread = new std::thread(&AudioCallSystem::startThread, this);
      return ;
    }
  while (!_exit)
    {
      tmp = 0;
      _mutex.lock();
      if (!this->_users.empty())
	for (it = this->_users.begin(); it != this->_users.end(); ++it)
	  {
	    if ((*it)->manager.get<sf::Clock *>("clock")->getElapsedTime().asMicroseconds() >=
		(*it)->manager.get<sf::Time *>("time")->asMicroseconds())
	      {
		try {
		  delete (*it)->manager.get<sf::SoundBuffer *>("toDelete");
		  (*it)->manager.remove<sf::SoundBuffer *>("toDelete");
		}
		catch (std::exception const &) {}
		if (!((*it)->manager.getAll<sf::SoundBuffer *>().empty()))
		  {
		    tmp = (*it)->manager.getAll<sf::SoundBuffer *>().front();
		    break ;
		  }
	      }
	  }
      if (tmp && *it)
	{
	  sound = (*it)->manager.get<sf::Sound *>("sound");
	  sound->stop();
	  sound->resetBuffer();
	  sound->setBuffer(*tmp);
	  sound->play();
	  (*it)->manager.get<sf::Clock *>("clock")->restart();
	  *((*it)->manager.get<sf::Time *>("time")) = tmp->getDuration();
	  (*it)->manager.remove<sf::SoundBuffer *>();
	  (*it)->manager.add<sf::SoundBuffer *>("toDelete", tmp);
	}
      _mutex.unlock();
    }
}

void AudioCallSystem::addBuffer(sf::SoundBuffer *buffer, const std::string &name)
{
  static unsigned int	id = 0;
  Entity	*tmp;
  std::vector <Entity *>::iterator it;

  for (it = this->_users.begin(); it != this->_users.end(); ++it)
    {
      if ((*it)->manager.get<std::string>("name") == name)
	break ;
    }
  if (it != this->_users.end())
    (*it)->manager.add<sf::SoundBuffer *>(std::to_string(id), buffer);
  else
    {
      tmp = new Entity;
      tmp->manager.add<std::string>("name", name);
      tmp->manager.add<sf::Clock *>("clock", new sf::Clock);
      tmp->manager.add<sf::Time *>("time", new sf::Time);
      tmp->manager.add<sf::Sound *>("sound", new sf::Sound);
      tmp->manager.add<sf::SoundBuffer *>(std::to_string(id), buffer);
      this->_users.push_back(tmp);
    }
  ++id;
}

void AudioCallSystem::addPacket(sf::SoundBuffer *buffer)
{
  IPacket *tmp = new UdpPacket();
  short int *data;
  const short int *tmpData;

  tmp->setSize(static_cast<uint16_t>(buffer->getSampleCount() * sizeof(short int)));
  data = new short int[static_cast<unsigned int>(buffer->getSampleCount())];
  tmp->setQuery(502);
  tmpData = buffer->getSamples();
  std::copy(tmpData, tmpData + buffer->getSampleCount(), data);
  tmp->setData(data);
  _packets.push_back(tmp);
  delete buffer;
  in(out());
}

void AudioCallSystem::in(IPacket *packet)
{
  sf::SoundBuffer *buffer = new sf::SoundBuffer();
  short int *tmpData;

  if (!packet)
    return ;
  tmpData = (short int *)(packet->getData());
  buffer->loadFromSamples(tmpData, packet->getSize() / sizeof(short int), 2, packet->getSize() / sizeof(short int));
  _mutex.lock();
  this->addBuffer(buffer, "toto");
  _mutex.unlock();
  return ;
}

IPacket *AudioCallSystem::out()
{
  IPacket *tmp;

  if (_packets.empty())
    return (0);
  tmp = _packets.front();
  _packets.pop_front();
  return (tmp);
}
