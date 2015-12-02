#include <algorithm>
#include "System/ISystem.hh"

void	EventAggregator::send(REvent e)
{
  for (auto x = _systemList.begin(); x != _systemList.end(); ++x)
    if (std::find(x->second.begin(), x->second.end(), e) != x->second.end())
      x->first->handle(e);
}

void	EventAggregator::add(REvent e, ISystem* s)
{
  if (std::find(_systemList[s].begin(), _systemList[s].end(), e)
      == _systemList[s].end())
    _systemList[s].push_back(e);
}

void	EventAggregator::update()
{
  std::vector<REvent> tmp;
  for (auto x = _systemList.begin(); x != _systemList.end(); ++x)
    {
      if ((tmp = x->first->broadcast()) != x->second)
	x->second = tmp;
    }
}
