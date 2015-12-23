#include "System/Render/ReadyScene.hh"
#include "System/Shoot/Pattern.hh"
#include "System/Collider/FCollision.hh"

ReadyScene::ReadyScene(IWindow &win, std::unordered_map<uint64_t, Entity*> *e):
    Scene(win, e), _isReady(false), _send(false),
    _new(false), _quit(false), _lastCode(0), _event(0)
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

void    ReadyScene::init()
{
    _isReady = false;
    _send = false;
    _new = false;
    _quit = false;
    _lastCode = 0;
    _event = 0;
    _isReadyText.setText("You are not ready.");
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

void    ReadyScene::in(IPacket *p, std::string &pseudo)
{
    TcpPacket   *packet;
    std::string data;
    std::string name;
    uint64_t id;
    std::string tmp;
    std::pair<float, float> pos(0, 1080 / 2);
    Entity *pl;

    if ((packet = dynamic_cast<TcpPacket*>(p)))
    {
        if (packet->getSize())
            data = std::string(static_cast<const char*>(packet->getData()),
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
                    _players.clear();
                    _quit = true;
                }
                break ;
            case Codes::Begin:
                _event = E_Stage;
                break ;
            case Codes::PlayerJoined:
                name = data.substr(0, data.find(":"));
                id = std::stoi(data.substr(data.find(":") + 1));
                pl = new Entity;
                pl->manager.add<std::string>("type", "player");
                pl->manager.add<int>("lifes", 10);
                pl->manager.add<std::string>("pseudo", name);
                pl->manager.add<std::string>("name", (name == pseudo ? "player1" : name));
                pl->manager.add<std::pair<float, float> >("position", pos);
                pl->manager.add<Pattern::Side>("direction", Pattern::Side::RIGHT);
                pl->manager.add<float>("velocity", 1.75f);
                pl->manager.add<Pattern::MovePattern>
                    ("pattern", Pattern::MovePattern::LINE);
                pl->manager.add<fCollision>("collision", &Collision::player);
                pl->manager.add<int>("force", 1);
                pl->manager.add<int>("shield", 0);
                pl->manager.add<int>("perfect_shield", 0);
                pl->manager.add<int>("respawn", 0);
                pl->manager.add<uint64_t>("score", 0);
                (*_entities)[id] = pl;
                (*_entities)[-1]->manager.set<uint64_t>("lastPlayer", id + 1);
                _players[name] = true;
                for (auto x : _players)
                    tmp += x.first + "\n";
                _playersText.setText(tmp);
                break ;
            case Codes::PlayerLeft:
                _players.erase(data);
                for (auto x : *_entities)
                    if (x.second->manager.get<std::string>("pseudo") == data)
                        _entities->erase(x.first);
                for (auto x : _players)
                    tmp += x.first + "\n";
                _playersText.setText(tmp);
                break;
            case Codes::PingLatency:
                _sendPongLatency = true;
                _packet.setQuery(static_cast<uint16_t>(Codes::PongLatency));
            default:
                ;
        }
    }
}

IPacket *ReadyScene::out(EventSum&)
{
    if (_sendPongLatency)
    {
         _sendPongLatency = false;
         return (&_packet);
    }
    if (_new)
    {
        _lastCode = _packet.getQuery();
        _new = false;
        return (&_packet);
    }
    return (0);
}
