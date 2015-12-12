#pragma once
# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  include "WinThread.hh"
# else
#  include "UnixThread.hh"
# endif

class ThreadFactory
{
public:
	ThreadFactory() {}
	~ThreadFactory() {}
	template <typename RET_VAL, typename ARG>
	static IThread<RET_VAL, ARG>* create()
	{
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
	return (new WinThread<RET_VAL, ARG>);
#else
	return (new UnixThread<RET_VAL, ARG>);
#endif
	}
};
