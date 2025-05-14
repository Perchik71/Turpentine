// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TGarbageCollector.h>
#include <TRelocation.h>
#include <TTime.h>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			DWORD WINAPI GarbageCollectorIntf::HKGarbageCollectorIntf_WaitForSingleObject
				(HANDLE hHandle, DWORD dwMilliseconds) noexcept(true)
			{
				if (dwMilliseconds == INFINITE)
				{
				repeat:
					auto Result = WaitForSingleObject(hHandle, 10);
					if (Result == WAIT_TIMEOUT)
					{
						SwitchToThread();
						goto repeat;
					}

					return Result;
				}

				return WaitForSingleObject(hHandle, dwMilliseconds);
			}

			static BOOL WINAPI HKSetThreadPriority_Critical(HANDLE Thread, int Priority) noexcept(true)
			{
				return SetThreadPriority(Thread, THREAD_PRIORITY_TIME_CRITICAL);
			}

			TTime f;

			static DWORD WINAPI HKWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)
			{
				auto time = f.CurrentTime;
				auto r = WaitForSingleObject(hHandle, dwMilliseconds);
				time = f.CurrentTime - time;
				if (time >= 1.)
					_MESSAGE("WaitForSingleObject %p %f", _ReturnAddress(), time);
				return r;
			}

			TTime d;

			static DWORD WINAPI HKWaitForSingleObjectEx(HANDLE hHandle, DWORD dwMilliseconds, BOOL bAlertable)
			{
				auto time = f.CurrentTime;
				auto r = WaitForSingleObjectEx(hHandle, dwMilliseconds, bAlertable);
				time = f.CurrentTime - time;
				if (time >= 1.)
					_MESSAGE("WaitForSingleObjectEx %p %f", _ReturnAddress(), time);
				return r;
			}

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

			TTime ThreadMemoryTimeTask;

			static DWORD WINAPI ThreadTask_WaitForSingleObject__Hook(ThreadMemoryTask* thread, DWORD dwMilliseconds) noexcept(true)
			{
				if (dwMilliseconds == INFINITE)
				{
					auto time = ThreadMemoryTimeTask.CurrentTime;
				repeat:
					auto Result = WaitForSingleObject(thread->hEvent, 50);
					if (Result == WAIT_TIMEOUT)
					{
						auto time_now = ThreadMemoryTimeTask.CurrentTime;
						
						if ((time_now - time) >= 1.f)
						{
							thread->Unlock();
							goto repeat;
						}

						time = time_now;
						
						SwitchToThread();
						goto repeat;
					}

					return Result;
				}

				return WaitForSingleObject(thread->hEvent, dwMilliseconds);
			}
		}

		void APIENTRY PatchGarbageCollector() noexcept(true)
		{
			*((uintptr_t*)&Impl::ptrCreateThreadTask) = REL::DetourJump(GlobalBase + 0xF63FA0, (uintptr_t)&Impl::CreateThreadTask);

			REL::Patch(GlobalBase + 0xF69FEA, { 0x48, 0x89, 0xF1, 0x90 });
			REL::DetourCall(GlobalBase + 0xF69FF0, (uintptr_t)&Impl::ThreadTask_WaitForSingleObject__Hook);

			// Return critical
			REL::DetourCall(GlobalBase + 0x6B20B42,
				(uintptr_t)&Impl::HKSetThreadPriority_Critical);


			//REL::DetourIAT(GlobalBase, "kernel32.dll", "WaitForSingleObject", (uintptr_t)&Impl::HKWaitForSingleObject);
			//REL::DetourIAT(GlobalBase, "kernel32.dll", "WaitForSingleObjectEx", (uintptr_t)&Impl::HKWaitForSingleObjectEx);
		}
	}
}