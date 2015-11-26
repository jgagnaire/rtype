#ifndef MENUSCENE_HH_
# define MENUSCENE_HH_

# include "Entity/Entity.hh"
# include "System/Render/Scene.hh"
# include "System/Render/View.hh"
# include "System/Render/Text.hh"
# include "System/Render/AnimatedSprite.hh"
# include "System/Render/ScrollingSprite.hh"

class MenuScene : public Scene
{
    public:
        MenuScene()
        {
            _entities.push_back(&_b1);
            _entities.push_back(&_b2);
            _entities.push_back(&_b3);
            _entities.push_back(&_b4);
            _entities.push_back(&_gui);

            _b1.manager.add<AView*>("view", &_v1);
            _s1.load("client/res/menu/menu.png");
            _b1.manager.add<ADrawable*>("s1", &_s1);
            _s2.load("client/res/menu/background-star-1.png", 3);
            _b2.manager.add<ADrawable*>("s2", &_s2);
            _s3.load("client/res/menu/background-star-2.png", 1);
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
            _s1.update();
            _s2.update();
            _s3.update();
            _s4.update();
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
};

#endif /* ifndef MENUSCENE_HH_ */
