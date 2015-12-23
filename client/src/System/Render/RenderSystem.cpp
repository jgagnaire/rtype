#ifndef RENDERSYSTEM_CPP_ZPTSKFN8
#define RENDERSYSTEM_CPP_ZPTSKFN8

# include "System/Render/RenderSystem.hh"
# include "System/Shoot/Pattern.hh"

RenderSystem::RenderSystem(std::unordered_map<uint64_t, Entity*> *e):
    _entities(e), _event(noEvent)
{
    _eventList.push_back(Key_Up);
    _eventList.push_back(Key_Down);
    _eventList.push_back(Key_Left);
    _eventList.push_back(Key_Right);
    _eventList.push_back(Key_Select);
    _eventList.push_back(Key_Close);
    _eventList.push_back(Key_Change);
    _eventList.push_back(E_Login);
    _eventList.push_back(E_Stage);
    _eventList.push_back(E_GameRoom);
    _eventList.push_back(E_Ready);
    _eventList.push_back(NewStage);
    _window = new Window();
    _menu = new MenuScene(*_window, _entities);
    _stage = new StageScene(*_window, _entities);
    _login = new LoginScene(*_window, _entities);
    _gameRoom = new GameRoomScene(*_window, _entities);
    _ready = new ReadyScene(*_window, _entities);
    _settings = new SettingsScene(*_window, _entities);
    _current = _menu;
}

RenderSystem::~RenderSystem()
{
    delete _window;
    delete _menu;
    delete _stage;
    delete _gameRoom;
}

void RenderSystem::update(int duration)
{
    _window->clear();
    _current->update(duration);
    _window->display();
}

IPacket *RenderSystem::out(EventSum &e)
{
    return _current->out(e);
}

void    RenderSystem::in(IPacket *p)
{
    _current->in(p, _pseudo);
}

bool RenderSystem::handle(EventSum e)
{
    switch (e)
    {
        case Key_Close:
            _window->close();
            break;
        case E_Login:
            _current = _login;
            break;
        case E_Stage:
            _current = _stage;
            break;
        case E_GameRoom:
            _current = _gameRoom;
            break;
        case E_Ready:
            _current = _ready;
            _current->init();
            break;
        case E_Settings:
            _current = _settings;
            break;
        case E_Menu:
            _current = _menu;
            break;
        default:
            _current->handle(e, _event);
    }
    if (_event == E_Stage)
    {
        _current = _stage;
        _current->init();
    }
    return true;
}

EventSum              RenderSystem::getEvent()
{
    EventSum          tmp = 0;

    if (_event != noEvent)
    {
        tmp = _event;
        _event = noEvent;
    }
    return tmp;
};

std::vector<REvent> &RenderSystem::broadcast(void)
{
    return _eventList;
}

IWindow         *RenderSystem::getWindow() const
{
    return _window;
}

#endif /* end of include guard: RENDERSYSTEM_CPP_ZPTSKFN8 */
