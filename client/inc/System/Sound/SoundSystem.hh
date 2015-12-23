#ifndef SOUND_SYSTEM_HPP
# define SOUND_SYSTEM_HPP

#include <SFML/Audio.hpp>
#include "System/ASystem.hh"
#include "Utility/RTime.hh"

class	SFX
{
public:
  SFX(const std::string& file) :_buffer(new SoundBuffer), _sound(new Sound)
  {
    _buffer->loadFromFile(file);
    _sound->setBuffer(*_buffer);
  }

  ~SFX()
  {
    delete (_buffer);
    delete (_sound);
  }

  void	play(void)
  {
    _sound->play();
  }

  void	stop(void)
  {
    _sound->stop();
  }

  void	setVolume(float vol)
  {
    _sound->setVolume(vol);
  }
private:
  ISoundBuffer		*_buffer;
  ISound		*_sound;
};

class AMusic
{
public:
	AMusic() {}
	virtual ~AMusic() {}

	virtual void	setTrack(const std::string &track) = 0;
	virtual void	play() = 0;
	virtual void	pause() = 0;
	virtual void	setVolume(float vol) = 0;
	virtual float	getVolume(void) const = 0;
	virtual void	repeat(bool r) = 0;
	virtual float	getRemainingTime() = 0;
	virtual bool	isRepeat() const = 0;
	virtual bool	isStop(void) const = 0;
protected:
	sf::Music		song;
};

class Music : public AMusic
{
public:
	Music() {}
	virtual ~Music() {}

	virtual void  setTrack(const std::string &track)
		{
			song.openFromFile(track);
		}

	virtual void  play()
		{
			song.play();
		}

	virtual void pause()
		{
			song.pause();
		}

	virtual void  setVolume(float vol)
		{
			song.setVolume(vol);
		}

	virtual float	getVolume(void) const
		{
			return (song.getVolume());
		}

	virtual void	repeat(bool r)
		{
			song.setLoop(r);
		}

	virtual float	getRemainingTime()
		{
			return (song.getDuration().asSeconds() - song.getPlayingOffset().asSeconds());
		}

	virtual bool	isRepeat() const
		{
			return (song.getLoop());
		}

	virtual bool isStop(void) const
		{
			return (song.getStatus() == sf::SoundSource::Status::Stopped);
		}

private:
  sf::Music		song;
};

class SoundSystem : public ASystem
{
public:
	SoundSystem() : typing("./client/res/sound/typing.flac"),
					fire("./client/res/sound/shoot.flac"),
					menuMove("./client/res/sound/menu_move.flac"),
					xplosion("./client/res/sound/explosion.flac"),
			idx(0), isActiv(false), isLogin(false), lvlIdx(0), musicLvl(50.0f), sfxLvl(70.0f)
		{
		    _eventList.push_back(Key_Up);
		    _eventList.push_back(Key_Down);
		    _eventList.push_back(Key_Left);
		    _eventList.push_back(Key_Right);
		    _eventList.push_back(Key_Select);
		    _eventList.push_back(Key_Fire);
		    _eventList.push_back(Key_Change);
		    _eventList.push_back(E_Login);
		    _eventList.push_back(E_Stage);
		    _eventList.push_back(E_GameRoom);
		    _eventList.push_back(E_Ready);
		    _eventList.push_back(E_MusicDown);
		    _eventList.push_back(E_MusicUp);
		    _eventList.push_back(E_SfxDown);
		    _eventList.push_back(E_SfxUp);
		    _eventList.push_back(NewStage);
		    Music *menu1 = new Music;
		    Music *menu2 = new Music;

		menu1->setTrack("./client/res/sound/menu_part1.flac");
		menu2->setTrack("./client/res/sound/menu_part2.flac");
		menu2->repeat(true);
		playlist.push_back(menu1);
		playlist.push_back(menu2);
		menu1->setVolume(musicLvl);
		menu1->play();
	}

	virtual ~SoundSystem() {}

	virtual void                    update(int)
		{
			if ((playlist.size() - idx) >= 1 && playlist[idx]->isStop()
				&& (playlist[idx]->isRepeat()) == false)
			{
				++idx;
				playlist[idx]->setVolume(musicLvl);
				playlist[idx]->play();
			}
		}
  virtual IPacket                 *out(EventSum &) {return NULL;}
  virtual void                    in(IPacket *p)
  {
    UdpPacket			*packet;
    TcpPacket			*tpacket;
    if ((packet = dynamic_cast<UdpPacket*>(p)) &&
	packet->getQuery() == static_cast<uint16_t>(UdpCodes::ServeKeyPressed))
      {
	std::string tmp = std::string(
				      static_cast<const char *>(packet->getData()), packet->getSize());
	std::string name = tmp.substr(0, tmp.find(":"));
	std::string code = tmp.substr(name.size() + 1);
	EventSum e = static_cast<EventSum>(std::atof(code.c_str()));
	if (e & Key_Fire)
	  {
	    fire.play();
	  }
      }
    else if ((tpacket = dynamic_cast<TcpPacket*>(p)))
      {
	std::string tmp = std::string(
				      static_cast<const char *>(tpacket->getData()), tpacket->getSize());
	if (p->getQuery() == static_cast<uint16_t>(Codes::JsonLevels))
	  {
	    Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("levels");
	    uint64_t			idx = 0;
	    for (auto &main : e.manager.getAll<Entity>())
	      {
		std::string music = main.second.manager.get<std::string>("music");
		Music *level = new Music;
		level->setTrack("./client/res/sound/" + music +".flac");
		level->setVolume(30);
		levels.push_back(level);
		++idx;
	      }
	  }
      }
  }
  virtual bool                    handle(EventSum e)
  {
    if (e == E_Login)
      isLogin = true;
    if (e == E_Ready)
        isActiv = false;
    if (e == E_Stage)
      {
	playlist[idx]->pause();
	levels[lvlIdx]->setVolume(musicLvl);
	levels[lvlIdx]->play();
	isLogin = false;
	isActiv = true;
      }
    else if (e == NewStage)
      {
	levels[lvlIdx]->pause();
	++lvlIdx;
	levels[lvlIdx]->setVolume(musicLvl);
	levels[lvlIdx]->play();
      }
    if (isLogin)
      {
	if (e & Key_Change)
	  typing.play();
      }
    else if (!isLogin && !isActiv)
      {
	if (e == Key_Up || e == Key_Down || e == Key_Left
	    || e == Key_Right || e == Key_Select)
	  menuMove.play();
	switch (e) {
	case (E_MusicDown) :
	  if (musicLvl > 0)
	    --musicLvl;
	  playlist[idx]->setVolume(musicLvl);
	  break ;
	case (E_MusicUp) :
	  if (musicLvl < 100)
	    ++musicLvl;
	  playlist[idx]->setVolume(musicLvl);
	  break ;
	case (E_SfxDown) :
	  if (sfxLvl > 0)
	    --sfxLvl;
	  break ;
	case (E_SfxUp) :
	  if (sfxLvl < 100)
	    ++sfxLvl;
	  break ;
	default :
	  ;
	}
	/*propifier avec un vector de SFX*/
	typing.setVolume(sfxLvl);
	xplosion.setVolume(sfxLvl);
	fire.setVolume(sfxLvl);
	menuMove.setVolume(sfxLvl);
      }
    if (isActiv && e == E_Explosion)
      xplosion.play();
    return true;
  }
	virtual std::vector<REvent>     &broadcast(void) { return _eventList; }
	virtual EventSum                getEvent() { return noEvent; }

protected:
	std::vector<REvent> _eventList;
	std::vector<AMusic*> playlist;
	std::vector<AMusic*> levels;
	SFX					typing;
	SFX					fire;
	SFX					menuMove;
	SFX					xplosion;
	int					idx;
	bool					isActiv;
	bool					isLogin;
	int					lvlIdx;
	float					musicLvl;
	float					sfxLvl;
};

#endif
