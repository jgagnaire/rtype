#ifndef FCOLLISION_HH_WPBKQ7AY
#define FCOLLISION_HH_WPBKQ7AY

# include "Entity/Entity.hh"

typedef std::function<bool (Entity&, Entity&)>   fCollision;

namespace Collision
{
    bool        explosion(Entity&, Entity &e);
    bool        player(Entity&, Entity &e);
    bool        shoot(Entity&, Entity &e);
    bool        mob(Entity&, Entity &e);
    bool        mobShoot(Entity&, Entity &e);
    bool        bonus(Entity&, Entity &e);
}

#endif /* end of include guard: FCOLLISION_HH_WPBKQ7AY */
