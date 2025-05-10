// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TThreads.h>
#include <TRelocation.h>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			BOOL WINAPI ThreadIntf::HKSetThreadPriority(HANDLE Thread, int Priority) noexcept(true)
			{
				// Don't allow a priority below normal - Fallout 4 doesn't have many "idle" threads
				return SetThreadPriority(Thread, max(THREAD_PRIORITY_NORMAL, Priority));
			}

			DWORD_PTR WINAPI ThreadIntf::HKSetThreadAffinityMask(HANDLE Thread, DWORD_PTR AffinityMask) noexcept(true)
			{
				// Don't change anything
				return numeric_limits<DWORD_PTR>::max();
			}
		}
	
		void APIENTRY PatchThreads() noexcept(true)
		{
			auto ProcessHandle = GetCurrentProcess();
			if (!SetPriorityClass(ProcessHandle, HIGH_PRIORITY_CLASS))
			{
				auto ErrorLast = GetLastError();
				_ERROR("SetPriorityClass returned failed (0x%x): %s", ErrorLast, _com_error(ErrorLast).ErrorMessage());
			}
			else
				_MESSAGE("Set high priority has been set for process");

			if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST))
			{
				auto ErrorLast = GetLastError();
				_ERROR("SetThreadPriority returned failed (0x%x): %s", ErrorLast, _com_error(ErrorLast).ErrorMessage());
			}
			else
				_MESSAGE("Set high priority has been set for main thread");

			DWORD_PTR processAffinityMask, systemAffinityMask;
			if (!GetProcessAffinityMask(ProcessHandle, &processAffinityMask, &systemAffinityMask))
			{
				auto ErrorLast = GetLastError();
				_ERROR("GetProcessAffinityMask returned failed (0x%x): %s", ErrorLast, _com_error(ErrorLast).ErrorMessage());
			}
			else
			{
				_MESSAGE("processAffinityMask: 0x%X", processAffinityMask);
				_MESSAGE("systemAffinityMask: 0x%X", systemAffinityMask);

				if (processAffinityMask != systemAffinityMask)
				{
					_MESSAGE("A change in the usage of processor cores has been detected");

					if (!SetProcessAffinityMask(ProcessHandle, systemAffinityMask))
					{
						auto ErrorLast = GetLastError();
						_ERROR("SetProcessAffinityMask returned failed (0x%x): %s", ErrorLast, _com_error(ErrorLast).ErrorMessage());
					}
					else
						_MESSAGE("Restore usage of processor cores");
				}

				// Complete removal of WinAPI functions SetPriorityClass and SetProcessAffinityMask.
				// Protection against premeditated, foolishly committed spoilage of the process.

				auto kernel_32 = GetModuleHandleA("kernel32.dll");
				if (kernel_32)
				{
					auto SetPriorityClass_addr = GetProcAddress(kernel_32, "SetPriorityClass");
					auto SetProcessAffinityMask_addr = GetProcAddress(kernel_32, "SetProcessAffinityMask");
					if (SetPriorityClass_addr)
					{
						REL::Patch((uintptr_t)SetPriorityClass_addr, { 0x31, 0xC0, 0xC3, 0x90, });
						REL::Patch((uintptr_t)SetProcessAffinityMask_addr, { 0x31, 0xC0, 0xC3, 0x90, });
					}
				}
			}

			REL::DetourIAT(GlobalBase, "kernel32.dll", "SetThreadPriority", (uintptr_t)&Impl::ThreadIntf::HKSetThreadPriority);
			REL::DetourIAT(GlobalBase, "kernel32.dll", "SetThreadAffinityMask", (uintptr_t)&Impl::ThreadIntf::HKSetThreadAffinityMask);

			// The system does not display the critical-error-handler message box. 
			// Instead, the system sends the error to the calling process.
			// Best practice is that all applications call the process - wide SetErrorMode 
			// function with a parameter of SEM_FAILCRITICALERRORS at startup.
			// This is to prevent error mode dialogs from hanging the application.
			DWORD OldErrMode = 0;
			if (!SetThreadErrorMode(SEM_FAILCRITICALERRORS, &OldErrMode))
			{
				auto ErrorLast = GetLastError();
				_ERROR("SetThreadErrorMode returned failed (0x%x): %s", ErrorLast, _com_error(ErrorLast).ErrorMessage());
			}

			DebugLog::flush();
		}
	}
}
