#pragma once

//[[noreturn]] void _AssertionFailed(const char * file, unsigned long line, const char * desc);
//[[noreturn]] void _AssertionFailed_ErrCode(const char * file, unsigned long line, const char * desc, unsigned long long code);
//[[noreturn]] void _AssertionFailed_ErrCode(const char * file, unsigned long line, const char * desc, const char * code);		

// based on the boost implementation of static asserts
//template <bool x> struct StaticAssertFailure;
//template <> struct StaticAssertFailure <true> { enum { a = 1 }; };
//template <int x> struct static_assert_test { };

#define __MACRO_JOIN__(a, b)		__MACRO_JOIN_2__(a, b)
#define __MACRO_JOIN_2__(a, b)		__MACRO_JOIN_3__(a, b)
#define __MACRO_JOIN_3__(a, b)		a##b
#define __PREPRO_TOKEN_STR2__(a)	#a
#define __PREPRO_TOKEN_STR__(a)		__PREPRO_TOKEN_STR2__(a)
#define __LOC__						__FILE__ "("__PREPRO_TOKEN_STR__(__LINE__)") : "

#define STATIC_ASSERT(a)	typedef static_assert_test <sizeof(StaticAssertFailure<(bool)(a)>)> __MACRO_JOIN__(static_assert_typedef_, __COUNTER__)
