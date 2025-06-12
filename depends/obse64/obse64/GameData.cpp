#include "GameData.h"

RelocPtr <TESDataHandler *> g_dataHandler(0x09474418);

TESDataHandler * TESDataHandler::GetSingleton()
{
	return *g_dataHandler;
}
