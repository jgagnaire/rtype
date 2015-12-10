#ifndef SCENE_HH_
# define SCENE_HH_

# include "IWindow.hh"
# include "Entity/Entity.hh"
# include "Network/IPacket.hh"

class Scene
{
    public:
        Scene(IWindow &win):
            _win(win)
        {}
        virtual ~Scene() {}

        virtual void update(int) = 0;
        virtual void handle(REvents, REvents&) = 0;

        virtual void in(IPacket*) {}
        virtual IPacket *out() {return 0;}

        virtual std::vector<Entity*>    getEntities()
        {
            return _entities;
        }

    protected:
        std::vector<Entity*>            _entities;
        IWindow                         &_win;
};

#endif /* ifndef SCENE_HH_ */
