#ifndef RENDERSYSTEM_CPP_ZPTSKFN8
#define RENDERSYSTEM_CPP_ZPTSKFN8

# include "System/Render/RenderSystem.hh"

RenderSystem::RenderSystem(std::list<Entity*> *e):
    _entities(e), _event(noEvent)
{
    _eventList.push_back(Key_Up);
    _eventList.push_back(Key_Down);
    _eventList.push_back(Key_Left);
    _eventList.push_back(Key_Right);
    _eventList.push_back(Key_Select);
    _eventList.push_back(Key_Close);
    _eventList.push_back(Key_Change);
    _eventList.push_back(E_PlayOffline);
    _eventList.push_back(E_GameRoom);
    _window = new Window();
    _menu = new MenuScene(*_window, _entities);
    _stage = new StageScene(*_window, _entities);
    _login = new LoginScene(*_window, _entities);
    _gameRoom = new GameRoomScene(*_window, _entities);
    _ready = new ReadyScene(*_window, _entities);
    _current = _login;
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

IPacket *RenderSystem::out()
{
    return _current->out();
}

void    RenderSystem::in(IPacket *p)
{
    _current->in(p);
}

bool RenderSystem::handle(EventSum e)
{
    switch (e)
    {
        case Key_Close:
            _window->close();
            break;
        case E_PlayOffline:
            _current = _stage;
            break;
        case E_GameRoom:
            _current = _gameRoom;
            break;
        default:
            _current->handle(e, _event);
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
