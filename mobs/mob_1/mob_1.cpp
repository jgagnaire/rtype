#include "Entity/Entity.hh"
#include "System/Shoot/Pattern.hh"

#if defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
#include <windows.h>

BOOL APIENTRY DllMain(HINSTANCE, DWORD, LPVOID)
{
    return TRUE;
}

extern "C" __declspec(dllexport) Entity *dl_entry_point(void)
#else
	extern "C"
	{
		Entity *dl_entry_point(void)
#endif
{
	Entity *e = new Entity;
    e->manager.add<std::string>("name", "mob1");
    e->manager.add<std::string>("type", "mob");
    e->manager.add<float>("velocity", 0.30f);
    e->manager.add("position",
            std::pair<float, float>(0, 0));
    e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
        ("pattern", Pattern::line);
    e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
    return e;
}
#if !defined(WIN32) && !defined(_WIN32) && !defined(WIN64) && !defined(_WIN64)
	}
#endif