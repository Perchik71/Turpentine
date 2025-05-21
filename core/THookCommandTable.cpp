// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <THookCommandTable.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace RE
	{
		CommandInfo* OldTableCommands = nullptr;
		CommandInfo* NewTableCommands = nullptr;

		void APIENTRY DumpCommandTable(const char* fname)
		{
			auto f = _fsopen(fname, "wt", _SH_DENYRW);
			if (f)
			{
				fputs("Oblivion Remastered Commands:\n", f);

				auto lamda_print = [](FILE* f, CommandInfo* table, size_t size)
					{
						size_t i = 0;
						for (; i < size; i++)
						{
							fprintf(f, "\"%s\" \"%s\" opcode (%x) %sdesc: \"%s\"\n",
								table[i].longName, table[i].shortName, table[i].opcode, 
								(table[i].needsParent ? "reference " : ""), table[i].helpText);

							for (size_t j = 0; j < table[i].numParams; j++)
								fprintf(f, "\t[Param #%u] \"%s\" id (%u)\n", j,
									table[i].params[j].typeStr, table[i].params[j].typeID);
						}
					};

				lamda_print(f, OldTableCommands, 0x171);
				lamda_print(f, NewTableCommands, 0x85);

				fflush(f);
				fclose(f);
			}
		}

		void APIENTRY ShowMessageHint(const char* message)
		{
			if (!message || !message[0])
				return;

			//OldTableCommands[0x59].console((void*)0, OldTableCommands[0x59].params, (void*)message, 0);
			return;
		}
	}

	namespace Hooks
	{
		void APIENTRY CommandTable() noexcept(true)
		{
			RE::OldTableCommands = (RE::CommandInfo*)REL::Offset(0x8FEC850);
			RE::NewTableCommands = (RE::CommandInfo*)REL::Offset(0x8FF3C40);
		}
	}
}