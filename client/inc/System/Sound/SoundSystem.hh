#ifndef SOUND_SYSTEM_HPP
# define SOUND_SYSTEM_HPP

#include <SFML/Audio.hpp>
#include "System/ASystem.hh"
#include "Utility/RTime.hh"

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
	SoundSystem() :idx(0), isActiv(false) {
		fire = new Music();
		fire->setTrack("./client/res/sound/shoot.flac");
		_eventList.push_back(Key_Fire);
		_eventList.push_back(E_Stage);
		Music *menu1 = new Music;
		Music *menu2 = new Music;

		menu1->setTrack("./client/res/sound/menu_part1.flac");
		menu2->setTrack("./client/res/sound/menu_part2.flac");
		menu2->repeat(true);
		playlist.push_back(menu1);
		playlist.push_back(menu2);
		menu1->play();
	}
	
	SoundSystem(std::list<Entity*> *) {}
	virtual ~SoundSystem() {}

	virtual void                    update(int)
		{
			if ((playlist.size() - idx) >= 1 && playlist[idx]->isStop()
				&& (playlist[idx]->isRepeat()) == false)
			{
				++idx;
				playlist[idx]->setVolume(70);
				playlist[idx]->play();
			}
		}
	virtual IPacket                 *out(EventSum &) {return NULL;}
	virtual void                    in(IPacket *p)
		{
			UdpPacket   *packet;
			
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
					std::cout << "fire" << std::endl;
					fire->setVolume(70);
					fire->play();
				}
			}
		}
	virtual bool                    handle(EventSum e)
		{
			if (e == E_Stage)
				isActiv = !isActiv;
			if (isActiv)
			{
				// if (e & Key_Fire)
				// {
				// 	std::cout << "fire" << std::endl;
				// 	fire->setVolume(70);
				// 	fire->play();
				// }
			}
			return true;
		}
	virtual std::vector<REvent>     &broadcast(void) { return _eventList; }
	virtual EventSum                getEvent() { return noEvent; }

protected:
	std::vector<REvent> _eventList;
	std::vector<AMusic*> playlist;
	AMusic				*fire;
	AMusic				*xplosion;
	AMusic				*menuMove;
	int					idx;
	bool				isActiv;
};

#endif
