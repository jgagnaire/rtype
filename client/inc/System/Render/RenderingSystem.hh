#ifndef RENDERING_SYSTEM_HH_
# define RENDERING_SYSTEM_HH_

# include "System/Render/MenuEntity.hh"

class RenderingSystem
{
    public:
        RenderingSystem();
        virtual ~RenderingSystem();

    private:
        Menu            _menu;
};

#endif /* ifndef RENDERING_SYSTEM_HH_ */
