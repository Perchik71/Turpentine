#include "GameScript.h"

RelocAddr <ParseFunction> DefaultParseFunction(0x06946DB0);
RelocAddr <_ExtractArgs> ExtractArgs(0x0662DCA0);

// wrap this to give it a static address
bool Cmd_Default_Parse(u32 numParams, const ParamInfo * paramInfo, ScriptLineBuffer * lineBuf, ScriptBuffer * scriptBuf)
{
	return DefaultParseFunction(numParams, paramInfo, lineBuf, scriptBuf);
}

RelocPtr <CommandInfo>	g_firstScriptCommand(0x08FAF210);
RelocPtr <CommandInfo>	g_firstConsoleCommand(0x08FB6600);
