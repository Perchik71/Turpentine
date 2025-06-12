#include <ShlObj.h>
#include <tlhelp32.h>
#include "obse64_common/Types.h"
#include "obse64_common/Errors.h"
#include "obse64_common/Log.h"
#include "obse64_common/obse64_version.h"
#include "obse64_common/Utilities.h"
#include "obse64_common/FileStream.h"
#include "obse64_common/CoreInfo.h"
#include "LoaderError.h"
#include "IdentifyEXE.h"
#include "Inject.h"
#include "Options.h"
#include "SigCheck.h"
#include <string>

int main(int argc, char ** argv)
{
	DebugLog::openRelative(CSIDL_MYDOCUMENTS, "\\My Games\\" SAVE_FOLDER_NAME "\\OBSE\\Logs\\obse64_loader.txt");
	//gLog.SetPrintLevel(IDebugLog::kLevel_FatalError);
	//gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

	SYSTEMTIME now;
	GetSystemTime(&now);

	_MESSAGE("OBSE loader: initialize (version = %d.%d.%d %08X %04d-%02d-%02d %02d:%02d:%02d, os = %s)",
		OBSE_VERSION_INTEGER, OBSE_VERSION_INTEGER_MINOR, OBSE_VERSION_INTEGER_BETA, LOADER_VERSION,
		now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
		getOSInfoStr().c_str());

	if(!g_options.Read(argc, argv))
	{
		PrintLoaderError("Couldn't read arguments.");
		g_options.PrintUsage();

		return 1;
	}

	if(g_options.m_optionsOnly)
	{
		g_options.PrintUsage();
		return 0;
	}

//	if(g_options.m_verbose)
//		gLog.SetPrintLevel(IDebugLog::kLevel_VerboseMessage);

	if(g_options.m_launchCS)
	{
		PrintLoaderError("The editor should be launched directly.");

		return 1;
	}

	// get process/dll names
	bool		dllHasFullPath = false;
	const char	* baseDllName = g_options.m_launchCS ? "obse64_editor" : "obse64";
	bool		usedCustomRuntimeName = false;

	std::string	procName;

	if(g_options.m_launchCS)
	{
		PrintLoaderError("The construction set appears to be a custom UE plugin built by Virtuos. If this is ever released then we'll see if it needs modification.");
		return 1;
	}
	else
	{
		procName = getConfigOption("Loader", "RuntimeName");
		if(!procName.empty())
		{
			_MESSAGE("using runtime name from config: %s", procName.c_str());
			usedCustomRuntimeName = true;
		}
		else
		{
			procName = "OblivionRemastered-Win64-Shipping.exe";

			// check to see if someone screwed up their install
			std::string appName = getRuntimeName();
			if(!_stricmp(appName.c_str(), procName.c_str()))
			{
				PrintLoaderError("You have renamed obse64_loader and have not specified the name of the runtime.");

				return 1;
			}
		}
	}

	const std::string & runtimeDir = getRuntimeDirectory();
	std::string procPath = runtimeDir + procName;

	if(g_options.m_altEXE.size())
	{
		procPath = g_options.m_altEXE;
		_MESSAGE("launching alternate exe (%s)", procPath.c_str());
	}

	_MESSAGE("procPath = %s", procPath.c_str());

	// check if the exe exists
	{
		FileStream	fileCheck;
		if(!fileCheck.open(procPath.c_str()))
		{
			DWORD err = GetLastError();
			if(err)
				_MESSAGE("exe open check error = %08X", err);

			bool msStore = false;

			if(err == ERROR_ACCESS_DENIED)
			{
				// this might be ms store
				std::string manifestPath = runtimeDir + "../../../appxmanifest.xml";	// assuming this is there

				if(fileCheck.open(manifestPath.c_str()))
				{
					msStore = true;
				}
			}

			if(msStore)
			{
				PrintLoaderError("You have the MS Store/Gamepass version of Oblivion Remastered, which is not compatible with OBSE64.");
			}
			else if(usedCustomRuntimeName)
			{
				PrintLoaderError("Couldn't find %s. You have customized the runtime name via OBSE64's .ini file, and that file does not exist. This can usually be fixed by removing the RuntimeName line from the .ini file.)", procName.c_str());
			}
			else
			{
				PrintLoaderError("Couldn't find %s.", procName.c_str());
			}

			return 1;
		}
	}

	_MESSAGE("launching: %s (%s)", procName.c_str(), procPath.c_str());

	if(g_options.m_altDLL.size())
	{
		baseDllName = g_options.m_altDLL.c_str();
		_MESSAGE("launching alternate dll (%s)", baseDllName);

		dllHasFullPath = true;
	}

	std::string		dllSuffix;
	ProcHookInfo	procHookInfo;

	// check exe version
	if(!IdentifyEXE(procPath.c_str(), g_options.m_launchCS, &dllSuffix, &procHookInfo))
	{
		_ERROR("unknown exe");

		if(usedCustomRuntimeName)
		{
			PrintLoaderError("You have customized the runtime name via OBSE64's .ini file. Version errors can usually be fixed by removing the RuntimeName line from the .ini file.");
		}

		return 1;
	}

	if(g_options.m_crcOnly)
		return 0;

	// build dll path
	std::string	dllPath;
	if(dllHasFullPath)
	{
		dllPath = baseDllName;
	}
	else
	{
		dllPath = runtimeDir + baseDllName + "_" + dllSuffix + ".dll";
	}

	_MESSAGE("dll = %s", dllPath.c_str());

	// check to make sure the dll exists
	{
		FileStream	tempFile;

		if(!tempFile.open(dllPath.c_str()))
		{
			PrintLoaderError(
				"Couldn't find OBSE64 DLL (%s).\n"
				"Either you have not installed OBSE64 correctly, or a new version of Oblivion Remastered has been released.\n"
				"Please make sure you have installed OBSE64 correctly and are running it from your Oblivion Remastered folder.\n"
				"If a game patch was released since you last ran the game, please check the website for updates.\n"
				"You have game version %d.%d.%d installed. The loader is not lying to you.\n"
				"Check your game installation if you are still confused.", dllPath.c_str(), procHookInfo.getVersionMajor(), procHookInfo.getVersionMinor(), procHookInfo.getVersionBuild());
			return 1;
		}
	}

	// check to make sure the dll makes sense
	{
		bool dllOK = false;
		u32 dllVersion = 0;

		HMODULE resourceHandle = (HMODULE)LoadLibraryEx(dllPath.c_str(), nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
		if(resourceHandle)
		{
			if(is64BitDLL(resourceHandle))
			{
				auto * version = (const OBSECoreVersionData *)getResourceLibraryProcAddress(resourceHandle, "OBSECore_Version");
				if(version)
				{
					dllVersion = version->runtimeVersion;

					if(	(version->dataVersion == OBSECoreVersionData::kVersion) &&
						(version->runtimeVersion == procHookInfo.packedVersion))
					{
						dllOK = true;
					}
				}
			}

			FreeLibrary(resourceHandle);
		}

		u32 trustError = 0;

		if(dllOK)
		{
			if(!CheckDLLSignature(dllPath, &trustError))
			{
				switch(trustError)
				{
					case CERT_E_UNTRUSTEDROOT:	// this one could be a user going in and messing with the root certs
					case CERT_E_CHAINING:
						PrintLoaderError("Your computer has an outdated or corrupt certificate database. This will likely cause many other issues on your machine and needs to be fixed to continue.");
						break;
				}

				dllOK = false;
			}
		}

		if(!dllOK)
		{
			VS_FIXEDFILEINFO info;
			std::string productName;
			std::string productVersion;

			if(GetFileVersion(dllPath.c_str(), &info, &productName, &productVersion))
			{
				_MESSAGE("OBSE64 DLL version");
				DumpVersionInfo(info);
				_MESSAGE("productName = %s", productName.c_str());
				_MESSAGE("productVersion = %s", productVersion.c_str());
			}
			else
			{
				_MESSAGE("couldn't get file version info");
			}

			PrintLoaderError(
				"Bad OBSE64 DLL (%s).\n"
				"Do not rename files; it will not magically make anything work.\n"
				"%08X %08X %08X", dllPath.c_str(), procHookInfo.packedVersion, dllVersion, trustError);

			return 1;
		}
	}

	// steam setup
	if(procHookInfo.procType == kProcType_Steam)
	{
		// ### might not be needed now that there's no intermediate launcher
		
		// same for standard and nogore
		const char * kAppID = (g_options.m_launchCS == false ? "2623190" : "???");

		// set this no matter what to work around launch issues
		SetEnvironmentVariable("SteamGameId", kAppID);

		if(g_options.m_skipLauncher)
		{
			SetEnvironmentVariable("SteamAppID", kAppID);
		}
	}

	// launch the app (suspended)
	STARTUPINFO			startupInfo = { 0 };
	PROCESS_INFORMATION	procInfo = { 0 };

	startupInfo.cb = sizeof(startupInfo);

	DWORD createFlags = CREATE_SUSPENDED;
	if(g_options.m_setPriority)
		createFlags |= g_options.m_priority;

	if(!CreateProcess(
		procPath.c_str(),
		NULL,	// no args
		NULL,	// default process security
		NULL,	// default thread security
		FALSE,	// don't inherit handles
		createFlags,
		NULL,	// no new environment
		NULL,	// no new cwd
		&startupInfo, &procInfo))
	{
		if(GetLastError() == 740)
		{
			PrintLoaderError("Launching %s failed (%d). Please try running obse64_loader as an administrator.", procPath.c_str(), GetLastError());
		}
		else
		{
			PrintLoaderError("Launching %s failed (%d).", procPath.c_str(), GetLastError());
		}

		return 1;
	}

	_MESSAGE("main thread id = %d", procInfo.dwThreadId);

	// set affinity if requested
	if(g_options.m_affinity)
	{
		_MESSAGE("setting affinity mask to %016I64X", g_options.m_affinity);

		if(!SetProcessAffinityMask(procInfo.hProcess, g_options.m_affinity))
		{
			_WARNING("couldn't set affinity mask (%08X)", GetLastError());
		}
	}

	bool	injectionSucceeded = false;
	u32		procType = procHookInfo.procType;

	// inject the dll
	switch(procType)
	{
	case kProcType_Steam:
	case kProcType_Normal:
	case kProcType_GOG:
		injectionSucceeded = InjectDLLThread(&procInfo, dllPath.c_str(), true, g_options.m_noTimeout);
		break;

	default:
		HALT("impossible");
	}

	// start the process if successful
	if(!injectionSucceeded)
	{
		PrintLoaderError("Couldn't inject DLL.");

		_ERROR("terminating process");

		TerminateProcess(procInfo.hProcess, 0);
	}
	else
	{
		_MESSAGE("launching");

		if(!ResumeThread(procInfo.hThread))
		{
			_WARNING("WARNING: something has started the runtime outside of obse64_loader's control.");
			_WARNING("OBSE64 will probably not function correctly.");
			_WARNING("Try running obse64_loader as an administrator, or check for conflicts with a virus scanner.");
		}

		if(g_options.m_waitForClose)
			WaitForSingleObject(procInfo.hProcess, INFINITE);
	}

	// clean up
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);

	return 0;
}
