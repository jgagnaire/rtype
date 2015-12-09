#include <algorithm>
#include "System/ISystem.hh"
#include "System/Render/IWindow.hh"

void	EventAggregator::send(REvents e)
{
    for (auto x = _systemList.begin(); x != _systemList.end(); ++x)
    {
        for (auto y : x->second)
        {
            if (e & y)
            {
                x->first->handle(e);
                break ;
            }
        }
    }
}

void	EventAggregator::add(REvent e, ISystem* s)
{
    if (std::find(_systemList[s].begin(), _systemList[s].end(), e)
            == _systemList[s].end())
        _systemList[s].push_back(e);
}

void    EventAggregator::add(ISystem *s)
{
    _systemList[s] = s->broadcast();
}

void	EventAggregator::update()
{
    std::vector<REvent> tmp;
    REvents              e;

    if ((e = this->win->getEvent()) != noEvent)
        this->send(e);
    for (auto x = _systemList.begin(); x != _systemList.end(); ++x)
    {
        if ((tmp = x->first->broadcast()) != x->second)
            x->second = tmp;
        this->send(x->first->getEvent());
    }
}
