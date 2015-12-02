#ifndef AUDIO_HH_
# define AUDIO_HH_

#include <iostream>
#include <thread>
#include <list>
#include "AudioBuffer.hh"

class Audio
{
public:
  Audio();
  ~Audio();

  void	startAudio();
  void addBuffer(sf::SoundBuffer *buffer, const std::string &name);

  static void	startThread(Audio *obj);

private:

  AudioBuffer *addClient(const std::string &);
  std::list <AudioBuffer *> _clients;
  std::thread	*_thread;
  bool		_exit;
};

#endif // !AUDIO_HH_
