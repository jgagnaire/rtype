#include <algorithm>
#include "AudioCallSystem.hh"
#include "Utility/Clock.hh"

#include <stdlib.h>

AudioCallSystem::AudioCallSystem():
	recorder(new Recorder(*this)), _thread(ThreadFactory::create<void, AudioCallSystem *>()), _exit(false)
{
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
  _mutex = new WinMutex;
#else
  _mutex = new UnixMutex;
#endif
  recorder->start();
  _thread->loadFunc(&AudioCallSystem::startThread);
  _thread->create(this);
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
  SoundBuffer *tmp;
  Sound *sound;

  while (!_exit)
    {
      tmp = 0;
      _mutex->lock();
      if (!this->_users.empty())
	for (it = this->_users.begin(); it != this->_users.end(); ++it)
	  {
	    if ((*it)->manager.get<Clock *>("clock")->getElapsedTimeMicro() >=
		(*it)->manager.get<RTime *>("time")->getTimeMicro())
	      {
		try {
		  delete (*it)->manager.get<SoundBuffer *>("toDelete");
		  (*it)->manager.remove<SoundBuffer *>("toDelete");
		}
		catch (std::exception const &) {}
		if (!((*it)->manager.getAll<SoundBuffer *>().empty()))
		  {
		    tmp = (*it)->manager.getAll<SoundBuffer *>().front();
		    break ;
		  }
	      }
	  }
      if (tmp && *it)
	{
	  sound = (*it)->manager.get<Sound *>("sound");
	  sound->stop();
	  sound->resetBuffer();
	  sound->setBuffer(*tmp);
	  sound->play();
	  (*it)->manager.get<Clock *>("clock")->restart();
	  *((*it)->manager.get<RTime *>("time")) = *tmp;
	  (*it)->manager.remove<SoundBuffer *>();
	  (*it)->manager.add<SoundBuffer *>("toDelete", tmp);
	}
      _mutex->unlock();
    }
}

void AudioCallSystem::addBuffer(SoundBuffer *buffer, const std::string &name)
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
    (*it)->manager.add<SoundBuffer *>(std::to_string(id), buffer);
  else
    {
      tmp = new Entity;
      tmp->manager.add<std::string>("name", name);
      tmp->manager.add<Clock *>("clock", new Clock);
      tmp->manager.add<RTime *>("time", new RTime);
      tmp->manager.add<Sound *>("sound", new Sound);
      tmp->manager.add<SoundBuffer *>(std::to_string(id), buffer);
      this->_users.push_back(tmp);
    }
  ++id;
}

void AudioCallSystem::addPacket(SoundBuffer *buffer)
{
  IPacket *tmp = new UdpPacket();
  void *data;
  const short int *tmpData;

  tmp->setSize(static_cast<uint16_t>(buffer->getSampleCount() * sizeof(short int)
				     + 4 * sizeof(char)));
  data = static_cast<short int *>(malloc(static_cast<std::size_t>(buffer->getSampleCount()) * sizeof(short int)
					 + 4 * sizeof(char)));
  tmp->setQuery(CODE_SEND_PACKET);
  tmpData = buffer->getSamples();
  char *tmpPseudo = new char[4];
  tmpPseudo[0] = 'l';
  tmpPseudo[1] = 'o';
  tmpPseudo[2] = 'l';
  tmpPseudo[3] = ':';
  std::copy(tmpPseudo, tmpPseudo + 4, static_cast<char *>(data));
  std::copy(tmpData, tmpData + buffer->getSampleCount(),
	    reinterpret_cast<short int *>(&((static_cast<char *>(data))[4])));
  tmp->setData(data);
  _packets.push_back(tmp);
  delete buffer;
  in(out());
}

std::string AudioCallSystem::getPseudo(const void *data, uint16_t packetSize) const
{
  int	size;
  char	*tmpPseudo;
  std::string pseudo;

  size = 0;
  while ((static_cast<char *>(const_cast<void *>(data)))[size] != ':' &&
	 (size * sizeof(char)) < packetSize)
    ++size;
  if ((size * sizeof(char)) < packetSize)
    {
      tmpPseudo = new char[size + 1];
      std::copy(static_cast<char *>(const_cast<void *>(data)),
	        static_cast<char *>(const_cast<void *>(data)) + size,
		tmpPseudo);
      tmpPseudo[size] = 0;
      pseudo = tmpPseudo;
      delete tmpPseudo;
      ++size;
    }
  else
    pseudo = "";
  return (pseudo);
}

void AudioCallSystem::in(IPacket *packet)
{
  SoundBuffer *buffer = new SoundBuffer();
  std::string pseudo;
  const void *tmpData;

  if (!packet || !dynamic_cast<UdpPacket *>(packet)
      || packet->getQuery() == CODE_RECEIVE_PACKET)
    return ;
  tmpData = packet->getData();
  pseudo = getPseudo(tmpData, packet->getSize());
  tmpData = &((static_cast<char *>(const_cast<void *>(tmpData)))[pseudo.length() + 1]);
  buffer->loadFromSamples(static_cast<short int *>(const_cast<void *>(tmpData)),
			  packet->getSize() / sizeof(short int), 2,
			  packet->getSize() / sizeof(short int));
  _mutex->lock();
  this->addBuffer(buffer, pseudo);
  _mutex->unlock();
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
