#include <algorithm>
#include "AudioCallSystem.hh"
#include "Utility/Clock.hh"

AudioCallSystem::AudioCallSystem():
	recorder(new Recorder(*this)), _thread(ThreadFactory::create<void, AudioCallSystem *>()), _exit(false)
{
  _eventList.push_back(Key_Sound);
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
  ISoundBuffer *tmp;
  ISound *sound;

  while (!_exit)
    {
      tmp = 0;
      _mutex.lock();
      if (!this->_users.empty())
	for (it = this->_users.begin(); it != this->_users.end(); ++it)
	  {
	    if ((*it)->manager.get<IClock *>("clock")->getElapsedTimeMicro() >=
		(*it)->manager.get<IRTime *>("time")->getTimeMicro())
	      {
		try {
		  delete (*it)->manager.get<ISoundBuffer *>("toDelete");
		  (*it)->manager.remove<ISoundBuffer *>("toDelete");
		}
		catch (std::exception const &) {}
		if (!((*it)->manager.getAll<ISoundBuffer *>().empty()))
		  {
		    tmp = (*it)->manager.getAll<ISoundBuffer *>().front();
		    break ;
		  }
	      }
	  }
      if (tmp && *it)
	{
	  sound = (*it)->manager.get<ISound *>("sound");
	  sound->stop();
	  sound->resetBuffer();
	  sound->setBuffer(*tmp);
	  sound->play();
	  (*it)->manager.get<IClock *>("clock")->restart();
	  *((*it)->manager.get<IRTime *>("time")) = *tmp;
	  (*it)->manager.remove<ISoundBuffer *>();
	  (*it)->manager.add<ISoundBuffer *>("toDelete", tmp);
	}
      _mutex.unlock();
    }
}

void AudioCallSystem::addBuffer(ISoundBuffer *buffer, const std::string &name)
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
    (*it)->manager.add<ISoundBuffer *>(std::to_string(id), buffer);
  else
    {
      tmp = new Entity;
      tmp->manager.add<std::string>("name", name);
      tmp->manager.add<IClock *>("clock", new Clock);
      tmp->manager.add<IRTime *>("time", new RTime);
      tmp->manager.add<ISound *>("sound", new Sound);
      tmp->manager.add<ISoundBuffer *>(std::to_string(id), buffer);
      this->_users.push_back(tmp);
    }
  ++id;
}

void AudioCallSystem::addPacket(SoundBuffer *buffer)
{
  IPacket *tmp = new UdpPacket();
  void *data;
  short int *tmpData;

  tmp->setSize(static_cast<uint16_t>(buffer->getSampleCount() * sizeof(short int)));
  data = new short int [buffer->getSampleCount()];
  tmp->setQuery(CODE_SEND_PACKET);
  tmpData = const_cast<short int *>(buffer->getSamples());
  std::copy(tmpData, tmpData + buffer->getSampleCount(), static_cast<short int *>(data));
  tmp->setData(data);
  _packets.push_back(tmp);
  delete buffer;
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
      delete[] tmpPseudo;
      ++size;
    }
  else
    pseudo = "";
  return (pseudo);
}

void AudioCallSystem::in(IPacket *packet)
{
  ISoundBuffer *buffer = new SoundBuffer();
  std::string pseudo;
  const void *tmpData;

  if (!packet || !dynamic_cast<UdpPacket *>(packet)
      || packet->getQuery() != CODE_RECEIVE_PACKET)
    return ;
  tmpData = packet->getData();
  pseudo = getPseudo(tmpData, packet->getSize());
  tmpData = &((static_cast<char *>(const_cast<void *>(tmpData)))[pseudo.length() + 1]);
  buffer->loadFromSamples(static_cast<short int *>(const_cast<void *>(tmpData)),
			  (packet->getSize() - (pseudo.length() + 1) * sizeof(char))  / sizeof(short int), 2,
			  (packet->getSize() - (pseudo.length() + 1) * sizeof(char))  / sizeof(short int));
  _mutex.lock();
  this->addBuffer(buffer, pseudo);
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
