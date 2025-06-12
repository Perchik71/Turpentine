#include "GameForms.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameExtraData.h"

RelocAddr <_LookupFormByID> LookupFormByID(0x0660EA00);

enum
{
	kSlot_Head = 0x1 << TESBipedModelForm::kPart_Head,
	kSlot_Hair = 0x1 << TESBipedModelForm::kPart_Hair,
	kSlot_UpperBody = 0x1 << TESBipedModelForm::kPart_UpperBody,
	kSlot_LowerBody = 0x1 << TESBipedModelForm::kPart_LowerBody,
	kSlot_Hand = 0x1 << TESBipedModelForm::kPart_Hand,
	kSlot_Foot = 0x1 << TESBipedModelForm::kPart_Foot,
	kSlot_RightRing = 0x1 << TESBipedModelForm::kPart_RightRing,
	kSlot_LeftRing = 0x1 << TESBipedModelForm::kPart_LeftRing,
	kSlot_Amulet = 0x1 << TESBipedModelForm::kPart_Amulet,
	kSlot_Weapon = 0x1 << TESBipedModelForm::kPart_Weapon,
	kSlot_BackWeapon = 0x1 << TESBipedModelForm::kPart_BackWeapon,
	kSlot_SideWeapon = 0x1 << TESBipedModelForm::kPart_SideWeapon,
	kSlot_Quiver = 0x1 << TESBipedModelForm::kPart_Quiver,
	kSlot_Shield = 0x1 << TESBipedModelForm::kPart_Shield,
	kSlot_Torch = 0x1 << TESBipedModelForm::kPart_Torch,
	kSlot_Tail = 0x1 << TESBipedModelForm::kPart_Tail,
	kSlot_UpperLower = kSlot_UpperBody | kSlot_LowerBody,
	kSlot_UpperLowerFoot = kSlot_UpperLower | kSlot_Foot,
	kSlot_UpperLowerHandFoot = kSlot_UpperLowerFoot | kSlot_Hand,
	kSlot_UpperLowerHand = kSlot_UpperLower | kSlot_Hand,
	kSlot_BothRings = kSlot_RightRing | kSlot_LeftRing,
	kSlot_UpperHand = kSlot_UpperBody | kSlot_Hand,

	kSlot_None = 0,
};

u32 TESBipedModelForm::SlotForMask(u32 mask)
{
	switch(mask) {
		case kSlot_Head: return kPart_Head;
		case kSlot_Hair: return kPart_Hair;
		case kSlot_UpperBody: return kPart_UpperBody;
		case kSlot_LowerBody: return kPart_LowerBody;
		case kSlot_Hand: return kPart_Hand;
		case kSlot_Foot: return kPart_Foot;
		case kSlot_RightRing: return kPart_RightRing;
		case kSlot_LeftRing: return kPart_LeftRing;
		case kSlot_Amulet: return kPart_Amulet;
		case kSlot_Weapon: return kPart_Weapon;
		case kSlot_BackWeapon: return kPart_BackWeapon;
		case kSlot_SideWeapon: return kPart_SideWeapon;
		case kSlot_Quiver: return kPart_Quiver;
		case kSlot_Shield: return kPart_Shield;
		case kSlot_Torch: return kPart_Torch;
		case kSlot_Tail: return kPart_Tail;
			// combinations
		case kSlot_UpperLower: return 18;
		case kSlot_UpperLowerFoot: return 19;
		case kSlot_UpperLowerHandFoot: return 20;
		case kSlot_UpperLowerHand: return 21;
		case kSlot_BothRings: return kPart_RightRing;
		case kSlot_UpperHand: return 22;
		case kSlot_None: return 255;
		default: return 0;
	}
}

u32 TESBipedModelForm::MaskForSlot(u32 slot)
{
	u32 mask = 0;
	switch(slot) {
		case kPart_Head: mask = kSlot_Head; break;
		case kPart_Hair: mask = kSlot_Hair; break;
		case kPart_UpperBody: mask = kSlot_UpperBody; break;
		case kPart_LowerBody: mask = kSlot_LowerBody; break;
		case kPart_Hand: mask = kSlot_Hand; break;
		case kPart_Foot: mask = kSlot_Foot; break;
		case kPart_RightRing: mask = kSlot_BothRings; break;
		case kPart_LeftRing: mask = kSlot_BothRings; break;
		case kPart_Amulet: mask = kSlot_Amulet; break;
		case kPart_Weapon: mask = kSlot_Weapon; break;
		case kPart_BackWeapon: mask = kSlot_BackWeapon; break;
		case kPart_SideWeapon: mask = kSlot_SideWeapon; break;
		case kPart_Quiver: mask = kSlot_Quiver; break;
		case kPart_Shield: mask = kSlot_Shield; break;
		case kPart_Torch: mask = kSlot_Torch; break;
		case kPart_Tail: mask = kSlot_Tail; break;
		case 18: mask = kSlot_UpperLower; break;
		case 19: mask = kSlot_UpperLowerFoot; break;
		case 20: mask = kSlot_UpperLowerHandFoot; break;
		case 21: mask = kSlot_UpperLowerHand; break;
		case 22: mask = kSlot_UpperHand; break;
		case 255: mask = kSlot_None; break;
		default: break;
	}
	return mask;
}

const char * GetFullName(TESForm * baseForm)
{
	if(baseForm)
	{
		TESFullName * fullName = baseForm->GetFullName();
		if(fullName && fullName->name.m_data)
		{
			if(fullName->name.m_dataLen)
				return fullName->name.m_data;
		}
	}

	return "<no name>";
}

TESFullName * TESForm::GetFullName()
{
	TESForm * form = this;
	TESFullName * fullName = NULL;

	auto * refr = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
	if(refr)
	{
		// deleted references will have a NULL base form, so check for that
		if(refr->baseForm)
		{
			// is it a mapmarker?
			if(refr->baseForm->typeID == kFormType_Stat)
			{
				auto * mapMarker = refr->extraList.Get<ExtraMapMarker>();
				if(mapMarker && mapMarker->data)
					fullName = &mapMarker->data->fullName;
			}
			else		// use base form
				form = refr->baseForm;
		}
	}
	else if(typeID == kFormType_Cell)	// some exterior cells inherit name of parent worldspace
	{
		// re-enable when TESObjectCELL is defined
#if 0
		TESObjectCELL * cell = DYNAMIC_CAST(this, TESForm, TESObjectCELL);
		if(cell && cell->worldSpace)
			if(!cell->fullName.name.m_data || !cell->fullName.name.m_dataLen)
				form = cell->worldSpace;
#endif
	}

	if(!fullName)
		fullName = DYNAMIC_CAST(form, TESForm, TESFullName);

	return fullName;
}
