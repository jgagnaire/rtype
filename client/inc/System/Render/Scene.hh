#ifndef SCENE_HH_
# define SCENE_HH_

# include "IWindow.hh"
# include "Entity/Entity.hh"

class Scene
{
    public:
        Scene(IWindow &win):
            _win(win)
        {}
        virtual ~Scene() {}

        virtual void update(int) = 0;
        virtual void handle(REvent, REvent&) = 0;

        virtual std::vector<Entity*>    getEntities()
        {
            return _entities;
        }

    protected:
        std::vector<Entity*>            _entities;
        IWindow                         &_win;
};

#endif /* ifndef SCENE_HH_ */
