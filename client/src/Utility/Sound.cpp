#include "Sound.hh"

void Sound::play()
{
  this->_sound.play();
}
void Sound::stop()
{
  this->_sound.stop();
}

void Sound::resetBuffer()
{
  this->_sound.resetBuffer();
}

void Sound::setBuffer(const SoundBuffer &buffer)
{
  this->_buffer.loadFromSamples(buffer.getSamples(), buffer.getSampleCount(),
			  2, buffer.getSampleRate());
  this->_sound.setBuffer(this->_buffer);
}
