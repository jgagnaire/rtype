#ifndef LOGINSCENE_HH_QX5LVF1U
#define LOGINSCENE_HH_QX5LVF1U

#include "Entity/Entity.hh"
#include "System/Render/Scene.hh"
#include "System/Render/View.hh"
#include "System/Render/Text.hh"
#include "Network/TcpSocket.hh"
#include "Network/NetworkManager.hh"

enum class LoginState
{
    NotConnected,
    Connecting,
    WaitResponse,
    Connected
};

class LoginScene : public Scene
{
    public:
        LoginScene(IWindow &win):
            Scene(win), _titleLogin("Login :"), _titlePassword("Password :"),
            _finish(LoginState::NotConnected), _event(noEvent)
    {
        _entities.push_back(&_b1);
        _entities.push_back(&_texts);

        _titleLogin.setCenter();
        _printedLogin.setCenter();
        _titlePassword.setCenter();
        _printedPassword.setCenter();
        _titleLogin.setY(200);
        _printedLogin.setY(400);
        _titlePassword.setY(600);
        _printedPassword.setY(800);
        _b1.manager.add<AView*>("view", &_view);
        _texts.manager.add<ADrawable*>("titleLogin", &_titleLogin);
        _texts.manager.add<ADrawable*>("printedLogin", &_printedLogin);
        _texts.manager.add<ADrawable*>("titlePassword", &_titlePassword);
        _texts.manager.add<ADrawable*>("printedPassword", &_printedPassword);
        _texts.manager.add<ADrawable*>("error", &_error);
        _currentStr = &_login;
        _currentText = &_printedLogin;
    }
        virtual ~LoginScene()
        {

        }

        virtual void    in(IPacket *p)
        {
            TcpPacket   *packet;
            if ((packet = dynamic_cast<TcpPacket*>(p)))
            {
                if (_lastCode == Codes::Login)
                {
                    switch (static_cast<Codes>(packet->getQuery()))
                    {
                        case Codes::Ok:
                            _event = E_GameRoom;
                            break ;
                        case Codes::WrongUserPass:
                            _error.setText("Wrong username or password");
                            break ;
                        case Codes::AlreadyLogin:
                            _error.setText("Already Loged in");
                            break ;
                        default:
                            ;
                    }
                }
                else if (_lastCode == Codes::Register)
                {
                    switch (static_cast<Codes>(packet->getQuery()))
                    {
                        case Codes::Ok:
                            _event = E_GameRoom;
                            break ;
                        case Codes::WrongUserPass:
                            _error.setText("Wrong username or password");
                            break ;
                        case Codes::AlreadyLogin:
                            _error.setText("Already Loged in");
                            break ;
                        default:
                            ;
                    }
                }
            }
        }

        virtual void    handle(REvents e, REvents &send)
        {
            if (_event)
            {
                send = _event;
                _event = noEvent;
                return ;
            }
            REvents tmp = (e << 1) >> 1;
            if (e == Key_Change || _finish == LoginState::WaitResponse)
                return ;
            if (e & Key_Change)
            {
                if (tmp == 127 && _login.empty() == false)
                    _currentStr->erase(_login.size() - 1, 1);
                if (tmp == 126 || tmp == 125)
                {
                    if (_currentStr == &_login)
                    {
                        _currentStr = &_password;
                        _currentText = &_printedPassword;
                    }
                    else
                    {
                        _lastCode = (tmp == 126 ? Codes::Login : Codes::Register);
                        _finish = LoginState::Connecting;
                        _buf = _login + ":" + _password;
                        _packet.setQuery(static_cast<uint16_t>(_lastCode));
                        _packet.setData(_buf.c_str());
                        _packet.setSize(_buf.size());
                    }
                    return ;
                }
                if (tmp != 127)
                    *_currentStr += static_cast<char>(tmp);
                _currentText->setText(*_currentStr);
                _currentText->setCenter();
                _currentText->setY((_currentStr == &_login ? 400 : 800));
            }
        }

        virtual IPacket *out()
        {
            if (_finish != LoginState::Connecting)
                return (0);
            _finish = LoginState::WaitResponse;
            return &_packet;
        }

        virtual void    update(int)
        {
            _win.setMenuMode(true);
        }

    private:
        View                    _view;
        std::string             _login;
        std::string             _password;
        Entity                  _b1;
        Entity                  _texts;
        Text                    _titleLogin;
        Text                    _printedLogin;
        Text                    _titlePassword;
        Text                    _printedPassword;
        Text                    _error;
        Text                    *_currentText;
        std::string             *_currentStr;
        LoginState              _finish;
        TcpPacket               _packet;
        std::string             _buf;
        Codes                   _lastCode;
        REvent                  _event;
};

#endif /* end of include guard: LOGINSCENE_HH_QX5LVF1U */
