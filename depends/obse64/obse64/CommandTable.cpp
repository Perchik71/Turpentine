#include "CommandTable.h"
#include "GameScript.h"
#include "Hooks_Script.h"
#include "obse64_common/Log.h"
#include "obse64_common/obse64_version.h"
#include "obse64_common/Errors.h"
#include "GameConsole.h"

#define HANDLER(x) x

static bool Cmd_Test_Execute(COMMAND_ARGS)
{
	return true;
}

static bool Cmd_TestArgs_Execute(COMMAND_ARGS)
{
	u32	arg;

	if(ExtractArgs(EXTRACT_ARGS, &arg))
	{
		Console_Print("testargcommand: %d", arg);
	}
	else
	{
		Console_Print("testargcommand: couldn't extract args");
	}

	return true;
}

static CommandInfo kTestCommand =
{
	"testcommand", "tcmd",
	0,
	"test command for obse",
	0,
	0, nullptr,

	HANDLER(Cmd_Test_Execute)
};

static ParamInfo kTestArgCommand_Params[] =
{
	{	"int", kParamType_Integer, 0 }
};

static CommandInfo kTestArgCommand =
{
	"testargcommand", "tacmd",
	0,
	"test argument command for obse",
	0,
	1, kTestArgCommand_Params,

	HANDLER(Cmd_TestArgs_Execute)
};

void ImportConsoleCommand(const char * name)
{
	for(u32 i = 0; i < kScript_NumConsoleCommands; i++)
	{
		auto & cmd = g_firstConsoleCommand[i];

		if(!_stricmp(name, cmd.name))
		{
			AddScriptCommand(cmd);

			return;
		}
	}

	HALT("couldn't find console cmd");
}

static bool Cmd_GetOBSEVersion_Execute(COMMAND_ARGS)
{
	// the classic OBSE version we are pretending to be
	*result = OBSE_VERSION_IMPL_INTEGER;

	if(IsConsoleMode())
	{
		Console_Print("OBSE64 version: %d.%d.%d, release idx %d, runtime %08X, reported version %d",
			OBSE_VERSION_INTEGER, OBSE_VERSION_INTEGER_MINOR, OBSE_VERSION_INTEGER_BETA,
			OBSE_VERSION_RELEASEIDX, RUNTIME_VERSION, OBSE_VERSION_IMPL_INTEGER);
	}

	return true;
}

static CommandInfo kCommandInfo_GetOBSEVersion =
{
	"GetOBSEVersion", "",
	0,
	"returns the installed version of OBSE",
	0,
	0, nullptr,

	Cmd_GetOBSEVersion_Execute,
};

#define ADD(n) extern CommandInfo kCommandInfo_##n; AddScriptCommand(kCommandInfo_##n)
#define ADD_RET(n, r) extern CommandInfo kCommandInfo_##n; AddScriptCommand(kCommandInfo_##n, r)

void AddScriptCommands()
{
	AddScriptCommand(kTestCommand);
	AddScriptCommand(kTestArgCommand);
	ADD(GetNumItems);
	ADD(GetInventoryItemType);
	ADD(IsKeyPressed);
	ADD(GetEquipmentSlotType);
	ADD(PrintToConsole);
	ADD_RET(GetActiveSpell, kRetnType_Form);
	ADD_RET(SetActiveSpell, kRetnType_Form);
	ADD(SquareRoot);
	ADD(Sin);
	ADD(Cos);
	ADD(Tan);
	ADD(ASin);
	ADD(ACos);
	ADD(ATan);
	ADD(Log);
	ADD(Exp);
	ADD(GetParentCell);
	ADD(Log10);
	ADD(Floor);
	ADD(Ceil);
	ADD(Abs);
	ADD(Rand);
	ADD(Pow);
	ADD(ATan2);
	ADD(Sinh);
	ADD(Cosh);
	ADD(Tanh);
	ADD(dSin);
	ADD(dCos);
	ADD(dTan);
	ADD(dASin);
	ADD(dACos);
	ADD(dATan);
	ADD(dATan2);
	ADD(dSinh);
	ADD(dCosh);
	ADD(dTanh);
	ADD_RET(GetInventoryObject, kRetnType_Form);
	ADD_RET(GetEquippedObject, kRetnType_Form);
	ADD(IsKeyPressed2);
	ADD(TapKey);
	ADD(HoldKey);
	ADD(ReleaseKey);
	ADD(HammerKey);
	ADD(AHammerKey);
	ADD(UnHammerKey);
	ADD(DisableKey);
	ADD(EnableKey);
	ADD(MoveMouseX);
	ADD(MoveMouseY);
	ADD(SetMouseSpeedX);
	ADD(SetMouseSpeedY);
	ADD(DisableMouse);
	ADD(EnableMouse);
	ADD(GetOBSEVersion);
	ImportConsoleCommand("SetGameSetting");
	ImportConsoleCommand("SetINISetting");
	ImportConsoleCommand("GetINISetting");
	ImportConsoleCommand("SetFog");
	ImportConsoleCommand("SetClipDist");
	ImportConsoleCommand("SetImageSpaceGlow");
	ImportConsoleCommand("ToggleDetection");
	ImportConsoleCommand("SetCameraFOV");
	ImportConsoleCommand("SexChange");
	ImportConsoleCommand("RefreshINI");
	ImportConsoleCommand("HairTint");
	ImportConsoleCommand("SetTargetRefraction");
	ImportConsoleCommand("SetTargetRefractionFire");
	ImportConsoleCommand("SetSkyParam");
	ImportConsoleCommand("RunMemoryPass");
	ImportConsoleCommand("ModWaterShader");
	ImportConsoleCommand("WaterShallowColor");
	ImportConsoleCommand("WaterDeepColor");
	ImportConsoleCommand("WaterReflectionColor");
	ImportConsoleCommand("SetGamma");
	ImportConsoleCommand("SetHDRParam");
}
