#ifndef LOGINSCENE_HH_QX5LVF1U
#define LOGINSCENE_HH_QX5LVF1U

#include "Entity/Entity.hh"
#include "System/Render/Scene.hh"
#include "System/Render/View.hh"
#include "System/Render/Text.hh"

class LoginScene : public Scene
{
    public:
        LoginScene(IWindow &win):
            Scene(win), _titleLogin("Login :"), _titlePassword("Password :"),
            _finish(false)
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
        _currentStr = &_login;
        _currentText = &_printedLogin;
    }
        virtual ~LoginScene()
        {

        }

        virtual void    handle(REvents e, REvents &)
        {
            if (e == Key_Change || _finish)
                return ;
            if (e & Key_Change)
            {
                REvents tmp = (e << 1) >> 1;
                if (tmp == 127 && _login.empty() == false)
                    _currentStr->erase(_login.size() - 1, 1);
                if (tmp == 126)
                {
                    if (_currentStr == &_login)
                    {
                        _currentStr = &_password;
                        _currentText = &_printedPassword;
                    }
                    else
                        _finish = true;
                    return ;
                }
                if (tmp != 127)
                    *_currentStr += static_cast<char>(tmp);
                _currentText->setText(*_currentStr);
                _currentText->setCenter();
                _currentText->setY((_currentStr == &_login ? 400 : 800));
            }
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
        Text                    *_currentText;
        std::string             *_currentStr;
        bool                    _finish;
};

#endif /* end of include guard: LOGINSCENE_HH_QX5LVF1U */
