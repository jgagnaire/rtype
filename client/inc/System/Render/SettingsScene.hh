#ifndef SETTINGSSCENE_HH_TS9YNAG4
#define SETTINGSSCENE_HH_TS9YNAG4

# include "Entity.hh"
# include "Scene.hh"
# include "View.hh"
# include "Text.hh"
# include "ScrollingSprite.hh"

class SettingsScene : public Scene
{
    public:
        SettingsScene(IWindow &win, std::unordered_map<uint64_t, Entity*> *e):
            Scene(win, e), _current(0)
        {
            _b1.manager.add<AView*>("view", &_v1);

            _textVec.push_back(new Text("Option1"));
            _textVec.push_back(new Text("Option2"));
            _textVec.push_back(new Text("Option3"));
            for (uint64_t i = 0; i < _textVec.size(); ++i)
            {
                _textVec[i]->setCenter();
                _textVec[i]->setY(400 + i * 100);
                _gui.manager.add<ADrawable*>("text" + std::to_string(i),
                        _textVec[i]);
            }
        }

        virtual ~SettingsScene() {
            for (auto x : _textVec)
                delete x;
        }

        virtual void handle(EventSum e, EventSum &send)
        {
            switch (e)
            {
                case Key_Up:
                    if (_current > 0)
                        --_current;
                    break;
                case Key_Down:
                    if (_current  + 1 < _textVec.size())
                        ++_current;
                    break;
                case Key_Left:
                    std::cout << "LOL LEFT" << std::endl;
                    break;
                case Key_Right:
                    std::cout << "LOL RIGTH" << std::endl;
                    break;
                default:
                    ;
            }
            for (auto x : _textVec)
                x->setColor(0xffffffff);
            _textVec[_current]->setColor(0xff0000ff);
            if (e != Key_Change && e & Key_Change)
            {
                EventSum tmp = (e << 1) >> 1;
                if (tmp == 126)
                    std::cout << "I CLICK ON RETURN LOL "<< std::endl;
                else if (tmp == 127)
                    send = E_Menu;
            }
        }

        virtual void update(int)
        {
            _win.setMenuMode(true);
            _win.draw(_b1);
            _win.draw(_gui);
        }

    private:
        Entity              _b1;
        Entity              _gui;

        View                _v1;

        std::vector<Text*>  _textVec;
        uint64_t         _current;
};



#endif /* end of include guard: SETTINGSSCENE_HH_TS9YNAG4 */
