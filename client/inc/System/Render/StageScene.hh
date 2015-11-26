#ifndef STAGESCENE_HH_0PY2FK7L
# define STAGESCENE_HH_0PY2FK7L

# include "System/Render/Scene.hh"
# include "System/Render/ScrollingSprite.hh"
# include "System/Render/View.hh"

class StageScene : public Scene
{
    public:
        StageScene()
        {
            _entities.push_back(&_b1);
            _entities.push_back(&_b2);
            _entities.push_back(&_b3);

            _b1.manager.add<AView*>("view", &_view);
            _b1.manager.add<ADrawable*>("background", &_s1);
            _b2.manager.add<ADrawable*>("background", &_s2);
            _b3.manager.add<ADrawable*>("background", &_s3);

            _s1.load("client/res/stages/stage1/background.png", 1);
            _s2.load("client/res/stages/stage1/middle.png", 2);
            _s3.load("client/res/stages/stage1/top.png", 3);
        }

        virtual ~StageScene()
        {

        }

        virtual void    update()
        {
            _s1.update();
            _s2.update();
            _s3.update();
        }
    private:
        Entity          _b1;
        Entity          _b2;
        Entity          _b3;

        View            _view;
        ScrollingSprite _s1;
        ScrollingSprite _s2;
        ScrollingSprite _s3;
};


#endif /* end of include guard: STAGESCENE_HH_0PY2FK7L */
