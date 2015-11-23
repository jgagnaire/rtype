#include <unistd.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include "Audio.hh"
#include "Recorder.hh"

int main()
{
  Audio	test;
  sf::SoundBuffer *buffer;
  Recorder recorder;

  test.startAudio();
  recorder.start();
  while (sf::SoundBufferRecorder::isAvailable())
    {
      if ((buffer = recorder.getBuffer()))
	{
	  test.addBuffer(buffer, "toto");
	}
    }
  return 0;
}
