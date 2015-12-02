#include "Audio.hh"

Audio::Audio()
{
  this->_exit = false;
  _thread = 0;
}

Audio::~Audio()
{
  this->_exit = true;
  this->_thread->join();
  _clients.clear();
  delete this->_thread;
}

void	Audio::startThread(Audio *obj)
{
  obj->startAudio();  
}

void	Audio::startAudio()
{
  std::list <AudioBuffer *>::iterator it;
  sf::SoundBuffer *tmp = 0;

  if (!_thread)
    {
      _thread = new std::thread(&Audio::startThread, this);
      return ;
    }
  while (!_exit)
    {
      for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
	  if ((tmp = (*it)->getBuffer()))
	    break ;
	}
      if (tmp && (*it)->canPlay())
	(*it)->play();
    }
}

AudioBuffer *Audio::addClient(const std::string &name)
{
  AudioBuffer *tmp = new AudioBuffer(name);
  this->_clients.push_back(tmp);
  return (tmp);
}

void Audio::addBuffer(sf::SoundBuffer *buffer, const std::string &name)
{
  std::list <AudioBuffer *>::iterator it;

  for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
      if ((*it)->getName() == name)
	break ;
    }
  if (it != this->_clients.end())
    (*it)->addBuffer(buffer);
  else
    this->addClient(name)->addBuffer(buffer);
}
