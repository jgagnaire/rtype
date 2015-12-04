#ifndef RENDERSYSTEM_CPP_ZPTSKFN8
#define RENDERSYSTEM_CPP_ZPTSKFN8

# include "System/Render/RenderSystem.hh"

RenderSystem::RenderSystem()
{
    _eventList.push_back(Key_Up);
    _eventList.push_back(Key_Down);
    _eventList.push_back(Key_Left);
    _eventList.push_back(Key_Right);
    _eventList.push_back(Key_Enter);
    _current = &_menu;
}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::update(IClock &e)
{
    _current->update(e.getElapsedTimeMilli());
    e.restart();
}

IPacket *RenderSystem::out()
{
    return 0;
}

bool RenderSystem::handle(REvent)
{
    return true;
}

std::vector<REvent> RenderSystem::broadcast(void)
{
    return _eventList;
}

#endif /* end of include guard: RENDERSYSTEM_CPP_ZPTSKFN8 */
