
#include "System/Render/ReadyScene.hh"


ReadyScene::ReadyScene(IWindow &win, std::list<Entity*> *e):
	Scene(win, e), _isReady(false), _send(false),
	_new(false), _quit(false), _lastCode(0),
	_event(0)
{
	_b1.manager.add<View*>("view", &_view);
	_texts.manager.add<ADrawable*>("isReady", &_isReadyText);
	_texts.manager.add<ADrawable*>("list", &_playersText);

	_isReadyText.setText("You are not ready.");
	_playersText.setY(100);
}

ReadyScene::~ReadyScene()
{}

void   ReadyScene::update(int)
{
	_win.setMenuMode(true);
	_win.draw(_b1);
	_win.draw(_texts);
}

void    ReadyScene::handle(EventSum e, EventSum &send)
{
	if (_event)
	{
		send = _event;
		_event = 0;
		return ;
	}
	if (_quit)
	{
		send = E_GameRoom;
		_quit = false;
		_isReady = false;
		_isReadyText.setText("You are not ready.");
		return ;
	}
	if (e != Key_Change && e & Key_Change)
	{
		EventSum tmp = (e << 1) >> 1;
		if (tmp == 126)
		{
			if (_isReady)
				_packet.setQuery(static_cast<uint16_t>(Codes::NotReady));
			else
				_packet.setQuery(static_cast<uint16_t>(Codes::Ready));
			_new = true;
		}
		else if (tmp == 127)
		{
			_packet.setQuery(static_cast<uint16_t>(Codes::LeaveRoom));
			_new = true;
		}
	}
}

void    ReadyScene::in(IPacket *p)
{
	TcpPacket   *packet;
	std::string name;
	std::string tmp;
	Entity *e;

	if ((packet = dynamic_cast<TcpPacket*>(p)))
	{
		if (packet->getSize())
			name = std::string(static_cast<const char*>(packet->getData()),
							   packet->getSize());
		switch (static_cast<Codes>(packet->getQuery()))
		{
		case Codes::Ok:
			if (static_cast<Codes>(_lastCode) == Codes::Ready
				|| static_cast<Codes>(_lastCode) == Codes::NotReady)
				_isReady = !_isReady;
			if (_isReady)
				_isReadyText.setText("You are ready");
			else
				_isReadyText.setText("You are not ready.");
			if (static_cast<Codes>(_lastCode) == Codes::LeaveRoom)
			{
				_quit = true;
			}
			break ;
		case Codes::Begin:
			e = new Entity();
			for (auto x : _players)
			{
				e->manager.add<std::string>(x.first, x.first);
				Entity *pl = new Entity;
				pl->manager.add<std::string>("type", "player");
				pl->manager.add<std::string>("pseudo", x.first);
				pl->manager.add<std::string>("name", x.first);
				// pl->manager.add<Pattern::MovePattern>
				// 	("pattern", Pattern::MovePattern::LINE);
				_entities->push_back(pl);
			}
			e->manager.add<std::string>("type", "playersData");
			e->manager.add<std::string>("name", "playersData");
			_entities->push_back(e);
			_event = E_Stage;
			break ;
		case Codes::PlayerJoined:
			_players[name] = true;
			for (auto x : _players)
				tmp += x.first + "\n";
			_playersText.setText(tmp);
			break ;
		case Codes::PlayerLeft:
			_players.erase(name);
			for (auto x : _players)
				tmp += x.first + "\n";
			_playersText.setText(tmp);
			break;
		default:
			;
		}
	}
}

IPacket *ReadyScene::out()
{
	if (_new)
	{
		_lastCode = _packet.getQuery();
		_new = false;
		return (&_packet);
	}
	return (0);
}
