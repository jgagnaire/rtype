#include <algorithm>
#include "ASystem.hh"
#include "IWindow.hh"

void	EventAggregator::send(EventSum e)
{
    for (auto x = _systemList.begin(); x != _systemList.end(); ++x)
    {
        for (auto y : x->second)
        {
            if (e == 0 || e & y)
            {
                x->first->handle(e);
                break ;
            }
        }
    }
}

void	EventAggregator::add(REvent e, ASystem* s)
{
    if (std::find(_systemList[s].begin(), _systemList[s].end(), e)
            == _systemList[s].end())
        _systemList[s].push_back(e);
}

void    EventAggregator::add(ASystem *s)
{
    _systemList[s] = s->broadcast();
}

void	EventAggregator::update()
{
    std::vector<REvent> tmp;
    EventSum              e;

    if ((e = this->win->getEvent()) != noEvent)
        this->send(e);
    for (auto x = _systemList.begin(); x != _systemList.end(); ++x)
    {
        if ((tmp = x->first->broadcast()) != x->second)
            x->second = tmp;
        this->send(x->first->getEvent());
    }
}
