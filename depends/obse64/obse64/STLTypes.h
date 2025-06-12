#pragma once

// STL from MSVC 17.4.14 LTSC (19.34.31948, _MSC_VER=1934)
// https://devblogs.microsoft.com/cppblog/msvc-toolset-minor-version-number-14-40-in-vs-2022-v17-10/

// using local definitions because actual vector (etc) layout changes in debug

// 18
template <typename T>
class STLVector
{
public:
	T	* first;	// start of buffer
	T	* last;		// last valid
	T	* end;		// end of allocation
};
