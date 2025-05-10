#ifndef __OBSE64_VERSION_H__
#define __OBSE64_VERSION_H__

// these have to be macros so they can be used in the .rc
#define OBSE_VERSION_INTEGER		0
#define OBSE_VERSION_INTEGER_MINOR	1
#define OBSE_VERSION_INTEGER_BETA	0
#define OBSE_VERSION_VERSTRING		"0, 0, 1, 0"
#define OBSE_VERSION_PADDEDSTRING	"0001"
#define OBSE_VERSION_RELEASEIDX		1

//	x-------	major
//	-xxx----	minor
//	----xxx-	build
//	-------x	sub

#define MAKE_EXE_VERSION_EX(major, minor, build, sub)	((((major) & 0xF) << 28) | (((minor) & 0xFFF) << 16) | (((build) & 0xFFF) << 4) | ((sub) & 0xF))
#define MAKE_EXE_VERSION(major, minor, build)			MAKE_EXE_VERSION_EX(major, minor, build, 0)

#define GET_EXE_VERSION_MAJOR(a)	(((a) & 0xF0000000) >> 28)
#define GET_EXE_VERSION_MINOR(a)	(((a) & 0x0FFF0000) >> 16)
#define GET_EXE_VERSION_BUILD(a)	(((a) & 0x0000FFF0) >> 4)
#define GET_EXE_VERSION_SUB(a)		(((a) & 0x0000000F) >> 0)

#define RUNTIME_TYPE_BETHESDA	0
#define RUNTIME_TYPE_GOG		1
#define	RUNTIME_TYPE_EPIC		2
#define	RUNTIME_TYPE_MSSTORE	3

#define RUNTIME_VERSION_0_411_140	MAKE_EXE_VERSION(0, 411, 140)	// 0x019B08C0	initial Steam release

#define PACKED_OBSE_VERSION		MAKE_EXE_VERSION(OBSE_VERSION_INTEGER, OBSE_VERSION_INTEGER_MINOR, OBSE_VERSION_INTEGER_BETA)

// information about the state of the game at the time of release
#define OBSE_TARGETING_BETA_VERSION	0
#define CURRENT_RELEASE_RUNTIME		RUNTIME_VERSION_0_411_140
#define CURRENT_RELEASE_OBSE_STR	"0.1.0"

#if GET_EXE_VERSION_SUB(RUNTIME_VERSION) == RUNTIME_TYPE_BETHESDA
#define SAVE_FOLDER_NAME "Oblivion Remastered"
#elif GET_EXE_VERSION_SUB(RUNTIME_VERSION) == RUNTIME_TYPE_GOG
#define SAVE_FOLDER_NAME "Oblivion Remastered GOG"
#elif GET_EXE_VERSION_SUB(RUNTIME_VERSION) == RUNTIME_TYPE_EPIC
#define SAVE_FOLDER_NAME "Oblivion Remastered EPIC"
#elif GET_EXE_VERSION_SUB(RUNTIME_VERSION) == RUNTIME_TYPE_MSSTORE
#define SAVE_FOLDER_NAME "Oblivion Remastered MS"
#else
#error unknown runtime type
#endif

#define LOADER_VERSION_1_0_0	MAKE_EXE_VERSION(1, 0, 0)
#define LOADER_VERSION			LOADER_VERSION_1_0_0

#endif /* __OBSE64_VERSION_H__ */
