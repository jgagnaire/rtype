#ifndef SCENE_HH_
# define SCENE_HH_

# include <list>
# include "IWindow.hh"
# include "Entity/Entity.hh"
# include "Network/IPacket.hh"

class Scene
{
    public:
        Scene(IWindow &win, std::unordered_map<std::size_t, Entity*> *entities):
            _win(win), _entities(entities)
        {}

        virtual void init() {}
        virtual ~Scene() {}

        virtual void update(int) = 0;
        virtual void handle(EventSum, EventSum&) = 0;

        virtual void in(IPacket*, std::string&) {}
        virtual IPacket *out(EventSum &) {return 0;}

        virtual void draw() {}
    protected:
        IWindow                                     &_win;
        std::unordered_map<std::size_t, Entity*>    *_entities;
};

#endif /* ifndef SCENE_HH_ */
