#include "GameObjects.h"
#include "obse64_common/Relocation.h"

RelocAddr <PlayerCharacter *> PlayerCharacterSingleton(0x09413B08);

PlayerCharacter * PlayerCharacter::Get()
{
	return PlayerCharacterSingleton;
}
