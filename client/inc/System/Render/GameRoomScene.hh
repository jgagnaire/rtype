#ifndef GAMEROOMSCENE_HH_ZNKJPAAS
#define GAMEROOMSCENE_HH_ZNKJPAAS

#include <unordered_map>
#include "Network/NetworkManager.hh"
#include "Network/TcpSocket.hh"
#include "Scene.hh"
#include "System/Render/Text.hh"
#include "System/Render/View.hh"

class GameRoomScene : public Scene
{
    public:
        GameRoomScene(IWindow &win):
            Scene(win), _lastTime(0), _update(true), _new(false)
    {
        _entities.push_back(&_b1);
        _entities.push_back(&_texts);

        _b1.manager.add<View*>("view", &_view);
        _texts.manager.add<ADrawable*>("buffer", &_bufferText);
    }

        virtual ~GameRoomScene()
        {}

        virtual void    update(int duration)
        {
            _win.setMenuMode(true);
            _lastTime += duration;
            if (_lastTime > 5000)
                _update = true;
        }

        virtual void    handle(EventSum e, EventSum&)
        {
            if (e & Key_Change && e != Key_Change)
            {
                EventSum tmp = (e << 1) >> 1;

                if (tmp == 127 && _buffer.empty() == false)
                    _buffer.erase(_buffer.size() - 1, 1);
                if (tmp < 125)
                    _buffer += static_cast<char>(tmp);
                _bufferText.setText(_buffer);
                _bufferText.setCenter();
                if (tmp == 126)
                {
                    _packet.setQuery(static_cast<uint16_t>(Codes::CreateRoom));
                    _packet.setData(_buffer.c_str());
                    _packet.setSize(_buffer.size());
                    _new = true;
                }
            }
        }

        virtual void    in(IPacket *p)
        {
            std::string tmp, name, nb;
            TcpPacket   *packet;

            if ((packet = dynamic_cast<TcpPacket*>(p)))
            {
                switch (static_cast<Codes>(packet->getQuery()))
                {
                    case Codes::Ok:
                    case Codes::AlreadyInRoom:
                    case Codes::NotLoggedIn:
                        break;
                    case Codes::ExistingRoom:
                        tmp = static_cast<const char*>(packet->getData());
                        name = tmp.substr(0, tmp.find(":") - 1);
                        nb = tmp.substr(tmp.find(":"));
                        std::cout << "Name " << name << " Nb " << nb << std::endl;
                        _rooms[name] = std::atoi(nb.c_str());
                    default:
                        ;
                }
            }

        }

        virtual IPacket *out()
        {
            if (_new)
            {
                _new = false;
                 return (&_packet);
            }
            if (_update)
            {
                _packet.setQuery(static_cast<uint16_t>(Codes::GetGameRooms));
                _packet.setData(0);
                _packet.setSize(0);
                _update = false;
                _lastTime = 0;
                return (&_packet);
            }
            return (0);
        }

    private:
        std::unordered_map<std::string, int>        _rooms;
        View                                        _view;
        Entity                                      _b1;
        Entity                                      _texts;
        TcpPacket                                   _packet;
        int                                         _lastTime;
        bool                                        _update;
        bool                                        _new;
        std::string                                 _buffer;
        Text                                        _bufferText;
};

#endif /* end of include guard: GAMEROOMSCENE_HH_ZNKJPAAS */
