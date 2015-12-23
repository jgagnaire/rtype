#include "Entity/Entity.hh"
#include "JSONParser.hh"
#include "ServerError.hh"

#if defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
#include <windows.h>

BOOL APIENTRY DllMain(HINSTANCE, DWORD, LPVOID)
{
    return TRUE;
}

extern "C" __declspec(dllexport)
#else
extern "C"
{
#endif
Entity *dl_entry_point(void)
{
  try {
    JSONParser::parseFile("entities/bonuses.json");
    return (new Entity(JSONParser::parse()->getEntity()));
  }
  catch(JSONException const &) { return (0); }
}
#if !defined(WIN32) && !defined(_WIN32) && !defined(WIN64) && !defined(_WIN64)
}
#endif
