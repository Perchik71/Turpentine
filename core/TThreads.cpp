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
#pragma pack(push, 1)
			class ThreadMemoryTask
			{
			public:
				char pad08[0x8];
				HANDLE hEvent;
				bool bManualReset;
				char pad19[0x1F];
				HANDLE hThread;

				[[nodiscard]] virtual bool InitLock(bool manualReset) noexcept(true);
				[[nodiscard]] virtual bool HasManualReset() noexcept(true);
				virtual bool Unlock() noexcept(true);
				virtual bool Lock() noexcept(true);

				virtual bool Task(DWORD Timeout, char a3);
				virtual bool Unk28();
				virtual bool Unk30();
				virtual bool Unk38();

				[[nodiscard]] virtual bool Suspend(bool suspend = true) noexcept(true)
				{
					return (suspend ? SuspendThread(hThread) : ResumeThread(hThread));
				}
			};
#pragma pack(pop)

			static bool CreateThreadTask(
				ThreadMemoryTask* self,
				__int64 a2,
				wchar_t* a3,
				unsigned int a4,
				unsigned int a5,
				__int64 a6) noexcept(true);

			decltype(&CreateThreadTask) ptrCreateThreadTask = nullptr;

			static bool CreateThreadTask(
				ThreadMemoryTask* self,
				__int64 a2,
				wchar_t* a3,
				unsigned int a4,
				unsigned int a5,
				__int64 a6) noexcept(true)
			{
				bool b = ptrCreateThreadTask(self, a2, a3, a4, a5, a6);
				if (b)
					SetThreadPriority(self->hThread, THREAD_PRIORITY_TIME_CRITICAL);
				return b;
			}

			void WINAPI ThreadIntf::HKSleep(DWORD dwMilliseconds) noexcept(true)
			{
				if (dwMilliseconds >= 20)
				{
					SleepEx(20, FALSE);
					return;
				}

				SleepEx(dwMilliseconds, FALSE);
			}

			void WINAPI ThreadIntf::HKSleepEx(DWORD dwMilliseconds, BOOL bAlertable) noexcept(true)
			{
				if (dwMilliseconds >= 20)
				{
					SleepEx(20, bAlertable);
					return;
				}

				SleepEx(dwMilliseconds, bAlertable);
			}

			BOOL WINAPI ThreadIntf::HKSetThreadPriority(HANDLE Thread, int Priority) noexcept(true)
			{
				auto OldPriority = GetThreadPriority(Thread);
				// Don't allow a priority below normal
				return SetThreadPriority(Thread, max(THREAD_PRIORITY_NORMAL, 
					(OldPriority >= Priority ? OldPriority : Priority)));
			}

			DWORD_PTR WINAPI ThreadIntf::HKSetThreadAffinityMask(HANDLE Thread, DWORD_PTR AffinityMask) noexcept(true)
			{
				// Don't change anything
				return numeric_limits<DWORD_PTR>::max();
			}

			BOOL WINAPI ThreadIntf::HKSetThreadGroupAffinity(HANDLE hThread, GROUP_AFFINITY* GroupAffinity,
				PGROUP_AFFINITY PreviousGroupAffinity) noexcept(true)
			{
				// Don't change anything
				return TRUE;
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
					auto SetThreadGroupAffinity_addr = GetProcAddress(kernel_32, "SetThreadGroupAffinity");
					if (SetPriorityClass_addr)
					{
						REL::Patch((uintptr_t)SetPriorityClass_addr, { 0x31, 0xC0, 0xC3, 0x90, });
						REL::Patch((uintptr_t)SetProcessAffinityMask_addr, { 0x31, 0xC0, 0xC3, 0x90, });
						REL::Patch((uintptr_t)SetThreadGroupAffinity_addr, { 0x31, 0xC0, 0xC3, 0x90, });
					}
				}
			}

			*((uintptr_t*)&Impl::ptrCreateThreadTask) = REL::DetourJump(GlobalBase + 0xF63FA0, (uintptr_t)&Impl::CreateThreadTask);

			//REL::DetourIAT(GlobalBase, "kernel32.dll", "Sleep", (uintptr_t)&Impl::ThreadIntf::HKSleep);
			//REL::DetourIAT(GlobalBase, "kernel32.dll", "SleepEx", (uintptr_t)&Impl::ThreadIntf::HKSleepEx);
			REL::DetourIAT(GlobalBase, "kernel32.dll", "SetThreadPriority", (uintptr_t)&Impl::ThreadIntf::HKSetThreadPriority);
			REL::DetourIAT(GlobalBase, "kernel32.dll", "SetThreadAffinityMask", (uintptr_t)&Impl::ThreadIntf::HKSetThreadAffinityMask);
			REL::DetourIAT(GlobalBase, "kernel32.dll", "SetThreadGroupAffinity", 
				(uintptr_t)&Impl::ThreadIntf::HKSetThreadGroupAffinity);

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
		}
	}
}
