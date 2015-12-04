#ifndef RENDERSYSTEM_HH_C8GDRQJ2
# define RENDERSYSTEM_HH_C8GDRQJ2

# include "System/ISystem.hh"
# include "System/Render/MenuScene.hh"
# include "System/Render/StageScene.hh"
# include "Window.hh"

class RenderSystem : public ISystem
{
    public:
        RenderSystem();
        virtual ~RenderSystem();

        virtual void update(IClock&);
        virtual IPacket *out();
        virtual bool handle(REvent e);
        virtual std::vector<REvent> broadcast(void);

        inline IWindow     *getWindow() const;

    private:
        std::chrono::time_point<std::chrono::steady_clock>  _last;
        Scene                                               *_current;
        MenuScene                                           _menu;
        StageScene                                          _stage;
        IWindow                                             *_window;
};

#endif /* end of include guard: RENDERSYSTEM_HH_C8GDRQJ2 */
