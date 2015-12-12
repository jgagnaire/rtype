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
            Scene(win), _buttons(3), _lastTime(0), _update(true), _new(false),
            _current(0), _currentR(0)
    {
        _entities.push_back(&_b1);
        _entities.push_back(&_texts);

        _buttons[0].setText("Random");
        _buttons[0].setPosition(300, 50);
        _buttons[1].setText("Join");
        _buttons[1].setCenter();
        _buttons[1].setY(50);
        _buttons[2].setText("Create");
        _buttons[2].setPosition(1300, 50);

        _b1.manager.add<View*>("view", &_view);
        _texts.manager.add<ADrawable*>("buffer", &_bufferText);
        _texts.manager.add<ADrawable*>("list", &_list);
        for (std::size_t i = 0; i < _buttons.size(); ++i)
            _texts.manager.add<ADrawable*>("button" + std::to_string(i),
                    &_buttons[i]);
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
            std::string text;

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
            if (e & Key_Left && _current > 0)
                --_current;
            else if (e & Key_Right && _current < _buttons.size() - 1)
                ++_current;
            for (std::size_t i = 0; i <  _buttons.size(); ++i)
                _buttons[i].setColor(0xffffffff);
            _buttons[_current].setColor(0xff0000ff);
            for (auto x : _rooms)
                text += x.first + "\t" + x.second + "/4\n";
            _list.setText(text);
        }

        virtual void    in(IPacket *p)
        {
            std::string tmp, name;
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
                        name = tmp.substr(0, tmp.find(":"));
                        _rooms[name] = tmp.substr(tmp.find(":") + 1, 1);
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
        std::vector<Text>                                   _buttons;
        std::unordered_map<std::string, std::string>        _rooms;
        View                                        _view;
        Entity                                      _b1;
        Entity                                      _texts;
        TcpPacket                                   _packet;
        int                                         _lastTime;
        bool                                        _update;
        bool                                        _new;
        std::string                                 _buffer;
        Text                                        _bufferText;
        Text                                        _list;
        std::size_t                                 _current;
        std::size_t                                 _currentR;
};

#endif /* end of include guard: GAMEROOMSCENE_HH_ZNKJPAAS */
