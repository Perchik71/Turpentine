// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TTime.h"

#ifdef _WIN32
#include <windows.h>

namespace Turpentine
{
	TTime::TTime()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		m_Freq = double(li.QuadPart);
		QueryPerformanceCounter(&li);
		m_Start = li.QuadPart;
	}

	double TTime::GetCurrent() const
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		auto Return = double(li.QuadPart - m_Start) / m_Freq;
		return Return;
	}
}
#else
namespace Turpentine
{
	TTime::TTime() : m_Start(std::chrono::high_resolution_clock::now())
	{}

	double TTime::GetCurrent()
	{
		auto end = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<double> Return = m_Start - end;
		return Return.count();
	}
}
#endif