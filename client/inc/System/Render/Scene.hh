#ifndef SCENE_HH_
# define SCENE_HH_

# include <list>
# include "IWindow.hh"
# include "Entity/Entity.hh"
# include "Network/IPacket.hh"

class Scene
{
    public:
        Scene(IWindow &win, std::list<Entity*> *entities):
            _win(win), _entities(entities)
        {}

        virtual void init() {}
        virtual ~Scene() {}

        virtual void update(int) = 0;
        virtual void handle(EventSum, EventSum&) = 0;

        virtual void in(IPacket*) {}
        virtual IPacket *out() {return 0;}

        virtual void draw() {}
    protected:
        IWindow                         &_win;
        std::list<Entity*>              *_entities;
};

#endif /* ifndef SCENE_HH_ */
