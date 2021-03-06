#include <algorithm>
#include "AudioCallSystem.hh"
#include "Utility/Clock.hh"
#include "Network/NetworkManager.hh"

AudioCallSystem::AudioCallSystem() : _isActive(false)
{
  _eventList.push_back(E_Stage);
  _eventList.push_back(Key_Sound);
  _recorder.start();
}

AudioCallSystem::~AudioCallSystem()
{
  _recorder.stop();
  this->_packets.clear();
}

void	AudioCallSystem::update(int)
{
  std::vector <Entity *>::iterator it;
  ISoundBuffer *tmp;
  ISound *sound;

  tmp = 0;
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
		tmp = (*it)->manager.getAll<ISoundBuffer *>()[0].second;
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
  IPacket *tmp;
  void *data;
  short int *tmpData;

  if (!buffer)
    return ;
  tmp = new UdpPacket();
  tmp->setSize(static_cast<uint16_t>(buffer->getSampleCount() * sizeof(short int)));
  data = new short int [buffer->getSampleCount()];
  tmp->setQuery(static_cast<uint16_t>(UdpCodes::CodeSendPacket));
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

  if (!dynamic_cast<UdpPacket *>(packet)
      || packet->getQuery() != static_cast<uint16_t>(UdpCodes::CodeReceivePacket))
    return ;
  tmpData = packet->getData();
  pseudo = getPseudo(tmpData, packet->getSize());
  tmpData = &((static_cast<char *>(const_cast<void *>(tmpData)))[pseudo.length() + 1]);
  buffer->loadFromSamples(static_cast<short int *>(const_cast<void *>(tmpData)),
			  (packet->getSize() - (pseudo.length() + 1) * sizeof(char))  / sizeof(short int), 2,
			  (packet->getSize() - (pseudo.length() + 1) * sizeof(char))  / sizeof(short int));
  this->addBuffer(buffer, pseudo);
  return ;
}

IPacket *AudioCallSystem::out(EventSum &)
{
  IPacket *tmp;

  this->addPacket(this->_recorder.getBuffer());
  if (_packets.empty())
    return (0);
  tmp = _packets.front();
  _packets.pop_front();
  return (tmp);
}
