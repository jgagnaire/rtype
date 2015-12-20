#ifndef FCOLLISION_HH_WPBKQ7AY
#define FCOLLISION_HH_WPBKQ7AY

# include "Entity/Entity.hh"

typedef std::function<bool (Entity&)>   fCollision;

namespace Collision
{
    bool        explosion(Entity &e);
    bool        player(Entity &e);
    bool        shoot(Entity &e);
    bool        mob(Entity &e);
    bool        mobShoot(Entity &e);
    bool        bonus(Entity &e);
}

#endif /* end of include guard: FCOLLISION_HH_WPBKQ7AY */
