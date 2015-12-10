#ifndef RENDERSYSTEM_CPP_ZPTSKFN8
#define RENDERSYSTEM_CPP_ZPTSKFN8

# include "System/Render/RenderSystem.hh"

RenderSystem::RenderSystem()
{
    _eventList.push_back(Key_Up);
    _eventList.push_back(Key_Down);
    _eventList.push_back(Key_Left);
    _eventList.push_back(Key_Right);
    _eventList.push_back(Key_Select);
    _eventList.push_back(Key_Close);
    _eventList.push_back(Key_Change);
    _eventList.push_back(E_PlayOffline);
    _window = new Window();
    _menu = new MenuScene(*_window);
    _stage = new StageScene(*_window);
    _login = new LoginScene(*_window);
    _current = _login;
}

RenderSystem::~RenderSystem()
{
    delete _window;
    delete _menu;
    delete _stage;
}

void RenderSystem::update(IClock &e)
{
    _window->clear();
    _current->update(e.getElapsedTimeMilli());
    e.restart();
    for (auto x : _current->getEntities())
        _window->draw(*x);
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

bool RenderSystem::handle(REvents e)
{
    switch (e)
    {
        case Key_Close:
            _window->close();
            break;
        case E_PlayOffline:
            _current = _stage;
            break;
        default:
            _current->handle(e, _event);
    }
    return true;
}

REvents              RenderSystem::getEvent()
{
    REvents          tmp = 0;

    if (_event != 0)
    {
        tmp = _event;
        _event = noEvent;
    }
    return tmp;
};

std::vector<REvent> RenderSystem::broadcast(void)
{
    return _eventList;
}

IWindow         *RenderSystem::getWindow() const
{
    return _window;
}

#endif /* end of include guard: RENDERSYSTEM_CPP_ZPTSKFN8 */
