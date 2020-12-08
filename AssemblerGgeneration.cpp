#include "AssemblerGgeneration.h"

namespace AsmGen
{
	void Gen::AssemblerHead()
	{
		outFile << ".586\n";
		outFile << "\t.model flat, stdcall\n";
		outFile << "\tincludelib libucrt.lib\n";
		outFile << "\tincludelib libucrt.lib\n";
		outFile << "\tincludelib kernel32.lib\n";
		outFile << "\tExitProcess PROTO :DWORD\n\n";
		outFile << "\tTAV_Libary\n";

		outFile << "\twrites PROTO :DWORD\n";		// заголовки функций из станлартонй библиотеки
		outFile << "\twrited PROTO :DWORD\n";
		outFile << "\tstrl PROTO :DWORD\n";
		outFile << "\tdegree PROTO :DWORD, :DWORD\n";
		outFile << "\tradical PROTO :DWORD, :DWORD\n";

		outFile << "\n.stack 4096\n";
	}
}
