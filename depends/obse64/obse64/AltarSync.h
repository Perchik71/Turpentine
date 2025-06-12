#pragma once

#include "UnrealObject.h"
#include "UnrealTypes.h"

class UTESForm;

// 178
class UTESSync : public UObject
{
public:
	TMap <u32, FString>		m_allForms;						// 28 uproperty
	TMap <FString, u8>		m_fileCompileIndexes;			// 78 uproperty
	TMap <FString, u8>		m_filedRuntimeCompileIndexes;	// C8 uproperty
	TMap <u32, UTESForm *>	m_dynamicForms;					// 118 uproperty
	TArray <void *>			m_unk;							// 168 members need virtual destruction, probably uobject?
};

static_assert(sizeof(UTESSync) == 0x178);

// 10
struct VAltarActorGUID
{
	// just a standard guid?
	u32	a;
	u32	b;
	u32	c;
	u32	d;
};

static_assert(sizeof(VAltarActorGUID) == 0x10);

// 18
class IVPairableItem
{
public:
	virtual void	Pairable_0();
	virtual void	Pairable_1();
	virtual void	Pairable_2();
	virtual void	Pairable_3();
	virtual void	Pairable_4();
	virtual void	Pairable_5();
	virtual void	Pairable_6();

//	void	** _vtbl;		// 00
	u8		paired;			// 08
	u8		pad09[7];		// 09
	void	* pairObject;	// 10
};

static_assert(sizeof(IVPairableItem) == 0x18);
