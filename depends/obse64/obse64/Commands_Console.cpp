#include "GameConsole.h"
#include "GameScript.h"

/* Print formatted string to Oblivion console
 * syntax: PrintToConsole fmtstring num1 num2 ...
 * shortname: printc
 *
 * Prints a formatted string to the Oblivion console, similar to how printf() works.
 * -Format notation is the same as MessageBox, so you can use %#.#f %g %e %%
 * -While this function technically accepts floats, you can use %g to print integers.  Again, how this works
 *  is probably similar to how MessageBox works (ie. same range limitations).
 * -The string can be up to 511 characters long, not including the null byte.
 * -You can pass up to 9 variables, but you don't have to pass any at all.
 */
bool Cmd_PrintToConsole_Execute(COMMAND_ARGS)
{
	char fmtstring[BUFSIZ];
	float f0 = 0, f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0, f7 = 0, f8 = 0;

	if (ExtractArgs(EXTRACT_ARGS, &fmtstring, &f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8))
	{
		Console_Print(fmtstring, f0, f1, f2, f3, f4, f5, f6, f7, f8);
	}

	return true;
}

ParamInfo kParams_StringFormat[10] =
{
	{"string", kParamType_String, 0},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1}
};

CommandInfo kCommandInfo_PrintToConsole =
{
	"PrintToConsole", "printc",
	0,
	"Print formatted string to console",
	0,
	10, kParams_StringFormat,
	Cmd_PrintToConsole_Execute
};
