#include "GameScript.h"
#include "ParamInfos.h"
#include <Windows.h>

// the original version of the input commands was sketchy and would not work now with a modern input system
// let alone the other weirdness
// so let's document it for now

class InputHook
{
public:
	bool Get(u32 keycode) { return false; }
	void Tap(u32 keycode) { }
	void Hold(u32 keycode) { }
	void Release(u32 keycode) { }
	void Hammer(u32 keycode) { }
	void AHammer(u32 keycode) { }
	void UnHammer(u32 keycode) { }
	void Disable(u32 keycode) { }
	void Enable(u32 keycode) { }

	void MouseX(u32 dx) { }
	void MouseY(u32 dy) { }
	void MouseSpeedX(u32 dx) {}
	void MouseSpeedY(u32 dy) {}
	void DisableMouse() { }
	void EnableMouse() { }

private:
	u32 RemapMiceKeycodes(u32 keycode)
	{
		// original was
		// if(keycode%256==255&&keycode<2048) keycode=255+(keycode+1)/256;
		// 
		// this remaps keycodes xFF where x = 0-7 in to 10x (the mice inputs)
		// but only the first 8 bytes (no wheel, buttons only)
		// and I don't remember why but am sure something relies on it

		if(((keycode & 0xFF) == 0xFF) && (keycode <= 0x7FF))
			keycode = 0x100 + (keycode >> 8);

		return keycode;
	}
};

static InputHook g_InputHook;

// deprecated

static bool Cmd_IsKeyPressed_Execute(COMMAND_ARGS)
{
	*result = 0;
	u32 keycode = 0;

	if(!ExtractArgs(EXTRACT_ARGS, &keycode)) return true;
	if(GetAsyncKeyState(keycode) & 0x8000) *result = 1;

	return true;
}

static bool Cmd_IsKeyPressed2_Execute(COMMAND_ARGS)
{
	*result = 0;
	u32 keycode = 0;

	if(!ExtractArgs(EXTRACT_ARGS, &keycode)) return true;

	*result = g_InputHook.Get(keycode);

	return true;
}

template <void (InputHook::* Fn)(u32)>
static bool InputHookFn(COMMAND_ARGS)
{
	*result = 0;
	u32 keycode = 0;

	if(!ExtractArgs(EXTRACT_ARGS, &keycode)) return true;

	(g_InputHook.*Fn)(keycode);

	return true;
}

static bool Cmd_DisableMouse_Execute(COMMAND_ARGS)
{
	*result = 0;

	g_InputHook.DisableMouse();

	return true;
}

static bool Cmd_EnableMouse_Execute(COMMAND_ARGS)
{
	*result = 0;

	g_InputHook.EnableMouse();

	return true;
}

CommandInfo kCommandInfo_IsKeyPressed =
{
	"IsKeyPressed", "ikp",
	0,
	"return if a virtual keycode is down or up",
	0,
	1, kParams_OneInt,
	Cmd_IsKeyPressed_Execute
};

CommandInfo kCommandInfo_IsKeyPressed2 =
{
	"IsKeyPressed2", "ikp2",
	0,
	"return if a dx scancode is down or up",
	0,
	1, kParams_OneInt,
	Cmd_IsKeyPressed2_Execute
};

CommandInfo kCommandInfo_TapKey =
{
	"TapKey", "tk",
	0,
	"Fakes a key press for one frame",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::Tap>
};

CommandInfo kCommandInfo_HoldKey =
{
	"HoldKey", "hk",
	0,
	"Fakes a key press indefinately",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::Hold>
};

CommandInfo kCommandInfo_ReleaseKey =
{
	"ReleaseKey", "rk",
	0,
	"Releases a key held down by HoldKey",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::Release>
};

CommandInfo kCommandInfo_HammerKey =
{
	"HammerKey", "hk",	// duplicate. only the first will be found
	0,
	"Fakes key presses in alternate frames",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::Hammer>
};

CommandInfo kCommandInfo_AHammerKey =
{
	"AHammerKey", "ahk",
	0,
	"Fakes key presses in alternate frames",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::AHammer>
};

CommandInfo kCommandInfo_UnHammerKey =
{
	"UnHammerKey", "uhk",
	0,
	"Releases a key being hammered by HammerKey or AHammerKey",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::UnHammer>
};

CommandInfo kCommandInfo_DisableKey =
{
	"DisableKey", "dk",
	0,
	"Prevents a player from using a key",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::Disable>
};

CommandInfo kCommandInfo_EnableKey =
{
	"EnableKey", "ek",
	0,
	"Reenables a key previously disabled with DisableKey",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::Enable>
};

CommandInfo kCommandInfo_MoveMouseX =
{
	"MoveMouseX", "mmx",
	0,
	"Fakes a mouse movement x pixels along the x axis",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::MouseX>
};

CommandInfo kCommandInfo_MoveMouseY =
{
	"MoveMouseY", "mmy",
	0,
	"Fakes a mouse movement x pixels along the y axis",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::MouseY>
};

CommandInfo kCommandInfo_SetMouseSpeedX =
{
	"SetMouseSpeedX", "smsx",
	0,
	"Moves the mouse x pixels per second along the x axis",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::MouseSpeedX>
};

CommandInfo kCommandInfo_SetMouseSpeedY =
{
	"SetMouseSpeedY", "smsy",
	0,
	"Moves the mouse x pixels per second along the y axis",
	0,
	1, kParams_OneInt,
	InputHookFn<&InputHook::MouseSpeedY>
};

CommandInfo kCommandInfo_DisableMouse =
{
	"DisableMouse", "dm",
	0,
	"Disables mouse x/y axis movement",
	0,
	0, nullptr,
	Cmd_DisableMouse_Execute
};

CommandInfo kCommandInfo_EnableMouse =
{
	"EnableMouse", "em",
	0,
	"Enables the mouse after it has been disabled by DisableMouse",
	0,
	0, nullptr,
	Cmd_EnableMouse_Execute
};
