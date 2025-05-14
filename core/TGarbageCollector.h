// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

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
				{ return (suspend ? SuspendThread(hThread) : ResumeThread(hThread)); }
			};
#pragma pack(pop)

			class GarbageCollectorIntf
			{
			public:
				static DWORD WINAPI HKGarbageCollectorIntf_WaitForSingleObject
					(HANDLE hHandle, DWORD dwMilliseconds) noexcept(true);
			};
		}

		void APIENTRY PatchGarbageCollector() noexcept(true);
	}
}