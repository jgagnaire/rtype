#ifndef RENDERSYSTEM_HH_C8GDRQJ2
# define RENDERSYSTEM_HH_C8GDRQJ2

# include "System/ASystem.hh"
# include "System/Render/MenuScene.hh"
# include "System/Render/StageScene.hh"
# include "System/Render/LoginScene.hh"
# include "System/Render/GameRoomScene.hh"
# include "Window.hh"

class RenderSystem : public ASystem
{
    public:
        RenderSystem(std::list<Entity*> *);
        virtual ~RenderSystem();

        virtual void                update(IClock&);
        virtual IPacket             *out();
        virtual void                in(IPacket*);
        virtual bool                handle(EventSum e);
        virtual std::vector<REvent> &broadcast(void);
        virtual EventSum             getEvent(void);
        IWindow                     *getWindow() const;

    private:
        std::chrono::time_point<std::chrono::steady_clock>  _last;
        std::list<Entity*>                                  *_entities;
        Scene                                               *_current;
        MenuScene                                           *_menu;
        StageScene                                          *_stage;
        LoginScene                                          *_login;
        GameRoomScene                                       *_gameRoom;
        IWindow                                             *_window;
        EventSum                                             _event;
};

#endif /* end of include guard: RENDERSYSTEM_HH_C8GDRQJ2 */
