#include "SteamInit.h"
#include "obse64_common/Utilities.h"
#include "obse64_common/Log.h"
#include <filesystem>
#include <cstdio>
#include <Windows.h>

void SteamInit(u32 appID)
{
	u32 enabled = 0;
	if(getConfigOption_u32("Steam", "Init", &enabled) && enabled)
	{
		_MESSAGE("attempting to initialize steam");

		std::filesystem::path runtimeDirectory(getRuntimeDirectory());
		std::filesystem::path steamworksDirectory = runtimeDirectory / "../../../Engine/Binaries/ThirdParty/Steamworks/Steamv153/Win64";

		// create steam_appid.txt if it doesn't exist
		std::filesystem::path appidPath = runtimeDirectory / "steam_appid.txt";

		FILE * appidFile = nullptr;
		if(!_wfopen_s(&appidFile, appidPath.c_str(), L"wx") && appidFile)	// open for write, but only if it doesn't exist
		{
			_MESSAGE("creating steam_appid.txt (%S)", appidPath.c_str());

			fprintf_s(appidFile, "%d", appID);
			fclose(appidFile);
		}

		// load Steam API DLL
		_MESSAGE("loading Steam API dll");
		HMODULE steamDLL = LoadLibraryW((steamworksDirectory / "steam_api64.dll").c_str());
		if(steamDLL)
		{
			_MESSAGE("Steam API dll loaded");

			typedef bool (* SteamAPI_Init)();

			SteamAPI_Init initProc = (SteamAPI_Init)GetProcAddress(steamDLL, "SteamAPI_Init");
			if(initProc)
			{
				bool result = initProc();
				if(result)
					_MESSAGE("steam initialized");
				else
					_WARNING("steam init error");
			}
			else
			{
				_WARNING("couldn't find SteamAPI_Init");
			}
			
			// leave refcount on dll
		}
		else
		{
			_WARNING("couldn't find Steam API dll");
		}
	}
}
