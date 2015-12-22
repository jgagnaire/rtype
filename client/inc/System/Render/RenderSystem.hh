#ifndef RENDERSYSTEM_HH_C8GDRQJ2
# define RENDERSYSTEM_HH_C8GDRQJ2

# include "System/ASystem.hh"
# include "System/Render/MenuScene.hh"
# include "System/Render/StageScene.hh"
# include "System/Render/LoginScene.hh"
# include "System/Render/GameRoomScene.hh"
# include "System/Render/SettingsScene.hh"
# include "System/Render/ReadyScene.hh"
# include "Window.hh"

class ReadyScene;

class RenderSystem : public ASystem
{
    public:
        RenderSystem(std::unordered_map<uint64_t, Entity*> *);
		RenderSystem() {}
        virtual ~RenderSystem();

        virtual void                update(int duration);
        virtual IPacket             *out(EventSum &);
        virtual void                in(IPacket*);
        virtual bool                handle(EventSum e);
        virtual std::vector<REvent> &broadcast(void);
        virtual EventSum             getEvent(void);
        IWindow                     *getWindow() const;

    private:
        std::chrono::time_point<std::chrono::steady_clock>  _last;
        std::unordered_map<uint64_t, Entity*>            *_entities;
        Scene                                               *_current;
        MenuScene                                           *_menu;
        StageScene                                          *_stage;
        LoginScene                                          *_login;
        GameRoomScene                                       *_gameRoom;
        ReadyScene                                          *_ready;
        SettingsScene                                       *_settings;
        IWindow                                             *_window;
        EventSum                                             _event;
        std::string                                         _pseudo;
};

#endif /* end of include guard: RENDERSYSTEM_HH_C8GDRQJ2 */
