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
            _entities.push_back(_background);
            _entities.push_back(_gui);


            _background.load("client/res/menu/background_1920.png");
            _background.manager.add<AView*>("view", &_backgroundView);
            _background.manager.add<ADrawable*>("background", &_backgroundSprite);

            _title.load("client/res/menu/rtype-title_835.png", true);
            _title.setPosition(sf::Vector2f(542, 100));
            gui.manager.add<ADrawable*>("title", &title);

            arrow.load("client/res/menu/arrow_334.png");
            arrow.setPosition(sf::Vector2f(1500, 900));
            gui.manager.add<ADrawable*>("tmp2", &arrow);
        }

        virtual ~MenuScene() {}

        virtual void update()
        {

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
