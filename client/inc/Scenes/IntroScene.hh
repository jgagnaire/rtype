#ifndef INTROSCENE_HH_
# define INTROSCENE_HH_

# include "Graphics/IDrawable.hh"
# include "AnimatedSprite.hh"

class IntroScene : public IDrawable
{
    public:
        IntroScene();
        virtual ~IntroScene();

        virtual void    update();

    private:
        AnimatedSprite      _arbok;
};

#endif /* ifndef INTROSCENE_HH_ */
