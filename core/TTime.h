// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace Turpentine
{
#ifdef _WIN32
	class TTime
	{
	public:
		TTime();
		virtual ~TTime() = default;
	public:
		double GetCurrent() const;
	public:
		PropertyReadOnly(GetCurrent) double CurrentTime;
	private:
		double m_Freq;
		__int64 m_Start;
	};
#else
#include <chrono>

	class TTime
	{
	public:
		TTime();
		virtual ~TTime() = default;
	public:
		double GetCurrent() const;
	public:
		PropertyReadOnly(GetCurrent) double CurrentTime;
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
#endif
}