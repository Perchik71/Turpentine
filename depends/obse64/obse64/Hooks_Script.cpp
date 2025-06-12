#include "Hooks_Script.h"
#include "obse64/GameConsole.h"
#include "obse64/GameScript.h"
#include "obse64/CommandTable.h"
#include "obse64_common/SafeWrite.h"
#include "obse64_common/obse64_version.h"
#include "obse64_common/BranchTrampoline.h"
#include "obse64_common/Relocation.h"
#include "obse64_common/Log.h"
#include "obse64_common/Errors.h"
#include "xbyak/xbyak.h"
#include <vector>
#include <map>

bool PadCommand_Execute(const ParamInfo * paramInfo,
	const char * scriptData,
	TESObjectREFR * thisObj,
	TESObjectREFR * containingObj,
	Script * script,
	ScriptLocals * locals,
	double * result,
	u32 * opcodeOffsetPtr)
{
	return true;
}

// 8
struct ParamTypeInfo
{
	u32		typeID;		// 0
	bool	variable;	// 4
	bool	isForm;		// 5
	u8		pad6[2];	// 6
};

// may need to patch/redirect this as well
RelocPtr <ParamTypeInfo> g_paramTypeInfo(0x08FB9030);

class HookedCommandTable
{
public:
	// duplicate the command table locally
	void	Init(u32 baseOpcode, const CommandInfo * ptr, u32 num);
	
	// pad out to new opcode
	void	Extend(u32 opcode);

	struct PatchInfo
	{
		u32	ptr;
		u32	offset;
		u32	type;
	};

	// copy to trampoline and apply code patches
	void	Apply(const PatchInfo * start, const PatchInfo * end, const PatchInfo * len);

	// number of commands registered (includes padding, vanilla, extended)
	size_t		NumCommands() const { return m_commands.size(); }

	CommandInfo *	GetByIdx(u32 idx) { return &m_commands[idx]; }
	CommandInfo *	GetByOpcode(u32 opcode) { return GetByIdx(opcode - m_baseOpcode); }

	void	Add(const CommandInfo & cmd, CommandReturnType retnType = kRetnType_Default);

	void	Lock() { m_locked = true; }

	void	Reserve(size_t len);

private:
	enum
	{
		kContext_Start = 0,
		kContext_End,
		kContext_Len,

		kContext_Num,
	};
	
	struct CmdExtraInfo
	{
		CommandReturnType	retnType = kRetnType_Default;
	};

	void	Apply(const PatchInfo * patch, uintptr_t baseValue, int context);

	std::vector <CommandInfo>	m_commands;
	std::vector <CmdExtraInfo>	m_cmdExtra;
	u32	m_baseOpcode;
	bool m_locked = false;

	// pointers in the trampoline to m_commands.begin() + key (bytes)
	// so we can do "mov reg, [reg + imm32]" to fetch them
	std::map <u8, void **>	m_trampolineAddresses[kContext_Num];
};

HookedCommandTable g_commandTable;

void AddScriptCommand(const CommandInfo & cmd, CommandReturnType retnType)
{
	g_commandTable.Add(cmd, retnType);
}

void HookedCommandTable::Add(const CommandInfo & cmd, CommandReturnType retnType)
{
	ASSERT(!m_locked);

	CommandInfo patchedCmd = cmd;

	patchedCmd.opcode = u32(m_baseOpcode + m_commands.size());
	if(!patchedCmd.parse) patchedCmd.parse = Cmd_Default_Parse;

	m_commands.push_back(patchedCmd);

	CmdExtraInfo extra;
	extra.retnType = retnType;

	m_cmdExtra.push_back(extra);
}

void HookedCommandTable::Reserve(size_t len)
{
	m_commands.reserve(len);
	m_cmdExtra.reserve(len);
}

void HookedCommandTable::Init(u32 baseOpcode, const CommandInfo * ptr, u32 num)
{
	m_baseOpcode = baseOpcode;

	m_commands.assign(ptr, ptr + num);

	CmdExtraInfo extra;
	m_cmdExtra.assign(num, extra);
}

void HookedCommandTable::Extend(u32 opcode)
{
	u32 lastOpcode = m_commands.back().opcode;

	CommandInfo pad =
	{
		"", "",
		0,
		"empty padding cmd",
		false,
		0, nullptr,

		PadCommand_Execute,
		Cmd_Default_Parse,
		nullptr,

		false, false
	};

	u32 commandsToAdd = opcode - lastOpcode + 1;
	Reserve(m_commands.size() + commandsToAdd);

	for(u32 i = lastOpcode + 1; i < opcode; i++)
		Add(pad);
}

/*** references to command table
 *	
 *	696E720	0D1D	base
 *			0D0C	num commands - 1
 *	large function, patch
 *	
 *	6972140	04AF	num commands
 *			04C2	base
 *	large function, patch
 *	
 *	698F100	006C	base
 *	simple patch
 *	
 *	6A35210	009B	base
 *			00A2	num commands
 *	simple patch
 *	
 *	696FC80	0066	short name
 *			006D	end command short name
 *			0122	opcode (load imagebase)
 *			0129	opcode
 *	simple patch
 *	
 *	6A29DA0	008A	num commands + 0x1000
 *			01B9	num commands + 0x1000
 *	simple patch
 *	
 *	6A70820	0003	num commands
 *			000D	base
 *	tiny function, replace
 *
 *	696FE00	0028	num commands
 *			0033	base
 *	replace function, it's small
 *
 *	6A70800	0000	num commands
 *			0011	num params
 *	tiny function, replace
 *	
 *	6A70870	0000	num commands
 *			0018	num params
 *			0025	params
 *	tiny function, replace
 *	
 *	698EDB0	0059	condition
 *			018B	base
 *	large function, patch
 *	reuse of load address register makes this complex
 *	condition stuff, could ignore for now
 *
 *	6A708C0	0026	num params
 *			0037	params
 *	medium function, patch
 *	reuse of load address register makes this complex
 *	more condition stuff
 *	
 *	6A709C0 0060	num params
 *			0094	num commands
 *			00BB	num params
 *			00CC	params
 *			016B	params
 *			01FB	num params
 *			020C	params
 *			02B5	num commands
 *	huge function with lots of references, patch
 *	reuse of load address register makes this complex
 *	more conditions
 *	
 *	all the annoying cases are conditions, look at that later
 *	
 */

enum CmdTablePatchType
{
	kPatchType_End = 0,

	// write two bytes
	kPatchType_Data16,

	// write four bytes
	kPatchType_Data32,

	// 7 byte "lea r64, offset32" instruction
	// will be converted to a 7 byte "mov r64, offset32" instruction
	// offset will be redirected to a pointer stored in the trampoline
	// different values of the 'offset' struct field will add and reference additional pointers
	kPatchType_LeaToLoadBase,

	// used to clear offset (instruction not parsed)
	// 8 byte "mov r9, offset[rbx + r9*8]" for example
	// 7 byte "mov eax, offset[rax + rcx*8]" also for example
	// or 
	kPatchType_WriteOffset32,

	// used when fetches of imagebase are used for multiple things
	// but the extra things are unused/debug only
	// must point at an 8 byte mov statement
	// "mov rbx, offset32[rbx + rcx*8]" changed to
	// "lea rbx, offset32; nop"
	kPatchType_FixupStringFetch
};

HookedCommandTable::PatchInfo kCmdTableStartPatches[] =
{
	{ 0x06942B70 + 0x0D0D + 0, 0, kPatchType_LeaToLoadBase },

	{ 0x06946570 + 0x04C2 + 0, 0, kPatchType_LeaToLoadBase },

	{ 0x06963520 + 0x006C + 0, 0, kPatchType_LeaToLoadBase },

	{ 0x06A095C0 + 0x009B + 0, 0, kPatchType_LeaToLoadBase },
	
	{ 0x069440C0 + 0x0066 + 0, offsetof(CommandInfo, shortName), kPatchType_LeaToLoadBase },
	{ 0x069440C0 + 0x0122 + 0, offsetof(CommandInfo, opcode), kPatchType_LeaToLoadBase },
	{ 0x069440C0 + 0x0129 + 3, 0, kPatchType_WriteOffset32 },

	{ 0, 0, kPatchType_End }
};

HookedCommandTable::PatchInfo kCmdTableEndPatches[] =
{
	{ 0x069440C0 + 0x006D + 0, offsetof(CommandInfo, shortName) + sizeof(CommandInfo), kPatchType_LeaToLoadBase },

	{ 0, 0, kPatchType_End }
};

HookedCommandTable::PatchInfo kCmdTableLenPatches[] =
{
	{ 0x06942B70 + 0x0CFC + 1, u32(-1), kPatchType_Data32 },

	{ 0x06946570 + 0x04AF + 2, u32(-1), kPatchType_Data32 },

	{ 0x06A095C0 + 0x00A2 + 1, 0, kPatchType_Data32 },

	{ 0x069FE150 + 0x008A + 2, 0x1000, kPatchType_Data32 },
	{ 0x069FE150 + 0x01B9 + 2, 0x1000, kPatchType_Data32 },

	{ 0, 0, kPatchType_End }
};

void HookedCommandTable::Apply(const PatchInfo * start, const PatchInfo * end, const PatchInfo * len)
{
	for(const auto * iter = start; iter->type != kPatchType_End; ++iter)
		Apply(iter, uintptr_t(&m_commands.front()), kContext_Start);

	for(const auto * iter = end; iter->type != kPatchType_End; ++iter)
		Apply(iter, uintptr_t(&m_commands.back()), kContext_End);

	for(const auto * iter = len; iter->type != kPatchType_End; ++iter)
		Apply(iter, m_commands.size(), kContext_Len);
}

void HookedCommandTable::Apply(const PatchInfo * patch, uintptr_t baseValue, int context)
{
	uintptr_t rebasedPtr = RelocationManager::s_baseAddr + patch->ptr;

	switch(patch->type)
	{
		case kPatchType_End:
			break;

		case kPatchType_Data16:
		{
			u16 newData = u16(baseValue + patch->offset);
			safeWrite16(rebasedPtr, newData);
		}
		break;

		case kPatchType_Data32:
		{
			u32 newData = u32(baseValue + patch->offset);
			safeWrite32(rebasedPtr, newData);
		}
		break;

		case kPatchType_LeaToLoadBase:
		{
			// simple bad disassembler
			u8 * buf = (u8 *)rebasedPtr;
			u8 rex = 0;

			u8 opcode = *buf++;

			if((opcode & 0xF0) == 0x40)
			{
				rex = opcode;
				ASSERT(rex & 0x08);	// assume 64 bit operand size

				opcode = *buf++;
			}

			u8 modrm = *buf++;

			// no sib/mode3
			ASSERT((modrm & 0x38) != 0x20);

			//_MESSAGE("LeaToLoadBase: %08X %02X %02X %02X", patch->ptr, rex, opcode, modrm);

			// get if it exists, insert empty entry if it doesn't
			auto trampolineAddressIter = m_trampolineAddresses[context].try_emplace(patch->offset, nullptr);
			if(trampolineAddressIter.second)
			{
				// fill the new entry
				void ** trampolinePtr = (void **)g_branchTrampoline.allocate();

				u8 * basePtr = nullptr;

				switch(context)
				{
					case kContext_Start:
						basePtr = (u8 *)&m_commands.front();
						break;

					case kContext_End:
						basePtr = (u8 *)&m_commands.back();
						break;

					default:
						ASSERT(false);
						break;
				}

				*trampolinePtr = basePtr + patch->offset;

				trampolineAddressIter.first->second = trampolinePtr;

				//_MESSAGE("allocated cmd table ptr %02X %016I64X (%016I64X)", patch->offset, trampolinePtr, *trampolinePtr);
			}

			// lea
			ASSERT(opcode == 0x8D);

			// MOV r64, r/m64
			safeWrite8(rebasedPtr + (rex ? 1 : 0), 0x8B);

			size_t displacementOffset = 2;
			if(rex) displacementOffset++;

			uintptr_t nextInstrAddr = rebasedPtr + displacementOffset + 4;
			ptrdiff_t requiredDisplacement = uintptr_t(trampolineAddressIter.first->second) - nextInstrAddr;
			ASSERT((requiredDisplacement >= INT_MIN) && (requiredDisplacement <= INT_MAX));

			safeWrite32(rebasedPtr + displacementOffset, u32(requiredDisplacement));
		}
		break;

		case kPatchType_WriteOffset32:
			safeWrite32(rebasedPtr, patch->offset);
			break;

		case kPatchType_FixupStringFetch:
			_MESSAGE("FixupStringFetch");
			break;
	}
}

bool IsScriptCmdParamAForm(u32 scriptCmdIdx, u32 paramTypeIdx)
{
	auto & cmdTable = g_commandTable;

	if(scriptCmdIdx >= cmdTable.NumCommands())
		return false;

	const auto * cmd = cmdTable.GetByIdx(scriptCmdIdx);
	if(paramTypeIdx >= cmd->numParams)
		return false;

	u32 typeID = cmd->params[paramTypeIdx].typeID;

	return g_paramTypeInfo[typeID].isForm;
}

bool IsScriptCmdParamARefr(u32 scriptCmdIdx, u32 paramTypeIdx)
{
	auto & cmdTable = g_commandTable;

	if(scriptCmdIdx >= cmdTable.NumCommands())
		return false;

	const auto * cmd = cmdTable.GetByIdx(scriptCmdIdx);
	if(paramTypeIdx >= cmd->numParams)
		return false;

	u32 typeID = cmd->params[paramTypeIdx].typeID;

	return (typeID == kParamType_ObjectRef) || (typeID == kParamType_Actor);
}

CommandInfo * GetCommandInfo(u32 opcode)
{
	u32 consoleCmdIdx = opcode - kScript_ConsoleOpBase;
	if(consoleCmdIdx < kScript_NumConsoleCommands)
		return &g_firstConsoleCommand[consoleCmdIdx];

	u32 scriptCmdIdx = opcode - kScript_ScriptOpBase;
	if(scriptCmdIdx < g_commandTable.NumCommands())
		return g_commandTable.GetByIdx(scriptCmdIdx);

	return nullptr;
}

u16 GetNumParameters(u32 cmdIdx)
{
	if(cmdIdx >= g_commandTable.NumCommands())
		return 0;

	return g_commandTable.GetByIdx(cmdIdx)->numParams;
}

RelocAddr <decltype(&IsScriptCmdParamAForm)> IsScriptCmdParamAForm_Original(0x06A44C10);
RelocAddr <decltype(&IsScriptCmdParamARefr)> IsScriptCmdParamARefr_Original(0x06A44BC0);
RelocAddr <decltype(&GetCommandInfo)> GetCommandInfo_Original(0x06944240);
RelocAddr <decltype(&GetNumParameters)> GetNumParameters_Original(0x06A44BA0);

void Hooks_Script_Apply()
{
	g_commandTable.Init(kScript_ScriptOpBase, g_firstScriptCommand, kScript_NumScriptCommands);
	g_commandTable.Extend(kScript_OBSEOpBase);

	AddScriptCommands();

	g_commandTable.Lock();

	g_branchTrampoline.write6Branch(IsScriptCmdParamAForm_Original.getUIntPtr(), uintptr_t(IsScriptCmdParamAForm));
	g_branchTrampoline.write6Branch(IsScriptCmdParamARefr_Original.getUIntPtr(), uintptr_t(IsScriptCmdParamARefr));
	g_branchTrampoline.write6Branch(GetCommandInfo_Original.getUIntPtr(), uintptr_t(GetCommandInfo));
	g_branchTrampoline.write6Branch(GetNumParameters_Original.getUIntPtr(), uintptr_t(GetNumParameters));

	g_commandTable.Apply(kCmdTableStartPatches, kCmdTableEndPatches, kCmdTableLenPatches);
}
