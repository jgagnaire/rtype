#ifndef MENUSCENE_HH_
# define MENUSCENE_HH_

# include "Entity/Entity.hh"
# include "System/Render/Scene.hh"
# include "System/Render/View.hh"
# include "System/Render/Text.hh"
# include "System/Render/AnimatedSprite.hh"

class MenuScene : public Scene
{
    public:
        MenuScene()
        {
            _entities.push_back(&_background);
            _entities.push_back(&_gui);

            _backgroundSprite.load("client/res/menu/background_1920.png");
            _background.manager.add<AView*>("view", &_backgroundView);
            _background.manager.add<ADrawable*>("background", &_backgroundSprite);

            _title.load("client/res/menu/rtype-title_835.png", true);
            _title.setPosition(sf::Vector2f(542, 100));
            _gui.manager.add<ADrawable*>("title", &_title);

            _arrow.load("client/res/menu/arrow_334.png");
            _arrow.setPosition(sf::Vector2f(1500, 900));
            _gui.manager.add<ADrawable*>("tmp2", &_arrow);

            _selector.load("client/res/menu/rect-select_450.png", true);
            _selector.setPosition(sf::Vector2f(735, 375));
            _gui.manager.add<ADrawable*>("selector", &_selector);

            _textVec.push_back(new Text("Play Online"));
            _textVec.push_back(new Text("Play Offline"));
            _textVec.push_back(new Text("Settings"));
            _textVec.push_back(new Text("Quit"));
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

        virtual void update()
        {
            _selector.update();
            _arrow.update();
            _title.update();
        }
    private:
        Entity              _background;
        Entity              _gui;

        View                _backgroundView;
        AnimatedSprite      _backgroundSprite;

        AnimatedSprite      _title;
        AnimatedSprite      _arrow;
        AnimatedSprite      _selector;
        std::vector<Text*>  _textVec;
};

#endif /* ifndef MENUSCENE_HH_ */
