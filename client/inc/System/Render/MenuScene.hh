#ifndef MENUSCENE_HH_
# define MENUSCENE_HH_

# include "Entity.hh"
# include "Scene.hh"
# include "View.hh"
# include "Text.hh"
# include "AnimatedSprite.hh"
# include "ScrollingSprite.hh"

class MenuScene : public Scene
{
    public:
        MenuScene(IWindow &win, std::list<Entity*> *e):
            Scene(win, e), _current(0)
        {
            _b1.manager.add<AView*>("view", &_v1);
            _s1.load("client/res/menu/menu.png");
            _b1.manager.add<ADrawable*>("s1", &_s1);
            _s2.load("client/res/menu/background-star-2.png", 3);
            _b2.manager.add<ADrawable*>("s2", &_s2);
            _s3.load("client/res/menu/background-star-1.png", 1);
            _b3.manager.add<ADrawable*>("s3", &_s3);
            _s4.load("client/res/menu/moon.png", 5);
            _b4.manager.add<ADrawable*>("s4", &_s4);

            _title.load("client/res/menu/rtype-title_835.png", true);
            _title.setPosition(sf::Vector2f(542, 100));
            _gui.manager.add<ADrawable*>("title", &_title);

            _arrow.load("client/res/menu/arrow_334.png");
            _arrow.setPosition(sf::Vector2f(1500, 900));
            _gui.manager.add<ADrawable*>("tmp2", &_arrow);

            _selector.load("client/res/menu/rect-select_450.png", true);
            _selector.setPosition(sf::Vector2f(735, 375));
            _gui.manager.add<ADrawable*>("selector", &_selector);

            _textVec.push_back(new Text("Play Online", 0xd12323ff));
            _textVec.push_back(new Text("Play Offline", 0xd12323ff));
            _textVec.push_back(new Text("Settings", 0xd12323ff));
            _textVec.push_back(new Text("Quit", 0xd12323ff));
            for (std::size_t i = 0; i < _textVec.size(); ++i)
            {
                _textVec[i]->setCenter();
                _textVec[i]->setY(400 + i * 100);
                _gui.manager.add<ADrawable*>("text" + std::to_string(i),
                        _textVec[i]);
            }
        }

        virtual ~MenuScene() {
            for (auto x : _textVec)
                delete x;
        }

        virtual void handle(EventSum e, EventSum &send)
        {
            switch (e)
            {
                case Key_Up:
                    if (_current > 0)
                    {
                        _selector.setPosition(_selector.getPosition() + sf::Vector2f(0, -100));
                        --_current;
                    }
                    break;
                case Key_Down:
                    if (_current  + 1 < _textVec.size())
                    {
                        _selector.setPosition(_selector.getPosition() + sf::Vector2f(0, 100));
                        ++_current;
                    }
                    break;
                default:
                    ;
            }
            if (e != Key_Change && e & Key_Change)
            {
                EventSum tmp = (e << 1) >> 1;
                if (tmp == 126)
                    switch (_current)
                    {
                        case 0:
                            send = E_Login;
                            break ;
                        case 1:
                            send = E_Stage;
                            break ;
                        case 2:
                            break ;
                        case 3:
                            send = Key_Close;
                            break ;
                    }
            }
        }

        virtual void update(int duration)
        {
            _win.setMenuMode(true);
            _selector.update(duration);
            _arrow.update(duration);
            _title.update(duration);
            _s1.update(duration);
            _s2.update(duration);
            _s3.update(duration);
            _s4.update(duration);
            _win.draw(_b1);
            _win.draw(_b2);
            _win.draw(_b3);
            _win.draw(_b4);
            _win.draw(_gui);
        }

    private:
        Entity              _b1;
        Entity              _b2;
        Entity              _b3;
        Entity              _b4;
        Entity              _gui;

        View                _v1;
        AnimatedSprite      _s1;
        ScrollingSprite     _s2;
        ScrollingSprite     _s3;
        ScrollingSprite     _s4;

        AnimatedSprite      _title;
        AnimatedSprite      _arrow;
        AnimatedSprite      _selector;
        std::vector<Text*>  _textVec;

        std::size_t         _current;
};

#endif /* ifndef MENUSCENE_HH_ */
