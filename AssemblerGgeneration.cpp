#include "AssemblerGgeneration.h"

namespace AsmGen
{
	Gen::Gen(LT::LexTable plextable, IT::IdTable pidTable, const char* pathOutFile)
	{
		lextable = plextable;
		idtable = pidTable;


		outFile = std::fstream(pathOutFile, std::ios::out | std::ios::app);
		if (!outFile.is_open())
			throw ERROR_THROW_IN(105, -1, -1);		
		AssemblerHead();
		AssemblerConst();
		//AssemblerData();
		//AssemblerCode();
		outFile.close();
	}
	void Gen::AssemblerHead()
	{
		outFile << ".586\n";
		outFile << "\t.model flat, stdcall\n";
		outFile << "\tincludelib libucrt.lib\n";
		outFile << "\tincludelib libucrt.lib\n";
		outFile << "\tincludelib kernel32.lib\n";
		outFile << "\tExitProcess PROTO :DWORD\n\n";
		outFile << "\tincludelib C:\\Users\\Anton\\source\\repos\\programming languages\\TAV_2020\\Debug\\TAV_2020_standartLibary.lib\n";

		outFile << "\tstrlength PROTO :DWORD\n";		// заголовки функций из станлартонй библиотеки
		outFile << "\tsubstr PROTO :DWORD\n";
		outFile << "\tstrl PROTO :DWORD\n";
		outFile << "\tstrcompare PROTO :DWORD, :DWORD\n";

		outFile << "\n.stack 4096\n";
	}
	void Gen::AssemblerConst()
	{																
		outFile << ".const\n";
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::L)
			{
				outFile << "\t" << idtable.table[i].id;
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR)
					outFile << " BYTE '" << idtable.table[i].values.vste->str << "', 0";
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
					outFile << " DWORD " << idtable.table[i].values.vint;
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
					outFile << " BYTE " << (int)idtable.table[i].values.vbool;
				outFile << '\n';
			}
		}
	}
	//void Gen::AssemblerData()
	//{
	//	outFile << ".data\n";
	//	for (int i = 0; i < lextable.size; i++)
	//	{
	//		if (lextable.table[i].lexema[0] == 'v')		// ???
	//		{
	//			if (idtable.table[lextable.table[i + 2].idxTI].idtype == 1)
	//			{
	//				outFile << "\t" << idtable.table[lextable.table[i + 2].idxTI].id;		// str int -> dword
	//				if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::STR)
	//					outFile << " DWORD ?";
	//				if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::INT)
	//					outFile << " DWORD ?";
	//				if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
	//					outFile << " BYTE ?";
	//				outFile << '\n';
	//			}
	//			for (int j = i; j < lextable.size; j++)
	//				lextable.table[j] = lextable.table[j + 4];
	//			lextable.size -= 4;
	//			i--;
	//		}
	//	}
	//}
	//void Gen::AssemblerCode()
	//{
	//	outFile << "\n.code\n";
	//	int 
	//		indOfProc = 0, indOflex = -1,	 // idx начала func,	idx kex
	//		rhesis = -1, rif = false,
	//		proc = false, head = false;		 // func flag,	

	//	for (int i = 0; i < lextable.size; i++)
	//	{
	//		if (lextable.table[i].lexema[0] == 'p')
	//		{
	//			indOfProc = i + 2;
	//			outFile << idtable.table[lextable.table[indOfProc].idxTI].id << " PROC ";
	//			proc = true;

	//			while (lextable.table[i].lexema[0] != ')')
	//			{
	//				rhesis = i++;
	//			}

	//			for (; lextable.table[i].lexema[0] != '('; i--)
	//			{
	//				if (lextable.table[i].lexema[0] == 'i')
	//				{
	//					if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT ||
	//						idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
	//					{
	//						outFile << idtable.table[lextable.table[i].idxTI].id << ":DWORD";
	//						if (lextable.table[i - 2].lexema[0] != '(')
	//							outFile << ", ";
	//					}
	//					if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
	//					{
	//						outFile << idtable.table[lextable.table[i].idxTI].id << ":BYTE";
	//						if (lextable.table[i - 2].lexema[0] != '(')
	//							outFile << ", ";
	//					}
	//				}
	//			}
	//			i = rhesis;
	//			outFile << std::endl;
	//		}
	//		// старт main proc
	//		if (lextable.table[i].lexema[0] == 'h')
	//		{
	//			head = true;
	//			outFile << "main PROC\n";
	//		}
	//		// завершение тела func
	//		if (lextable.table[i].lexema[0] == '}')
	//		{
	//			if (proc)
	//			{
	//				outFile << idtable.table[lextable.table[indOfProc].idxTI].id << " ENDP\n\n";
	//				proc = false;
	//			}
	//			else
	//				outFile << "call ExitProcess\nmain ENDP\n";
	//			indOfProc = 0;
	//		}
	//		// ret обработка
	//		if (lextable.table[i].lexema[0] == 'r')
	//		{
	//			if (head)
	//				outFile << "push 0\n";
	//			else
	//			{
	//				if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::INT)
	//					outFile << "\tmov eax, " << idtable.table[lextable.table[i + 1].idxTI].id << "\n\tret\n";
	//				if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
	//					outFile << "\tmov eax, offset " << idtable.table[lextable.table[i + 1].idxTI].id << "\n\tret\n";
	//			}
	//		}
	//		// вывод в консоль
	//		if (lextable.table[i].lexema[0] == 'y')
	//		{
	//			if (lextable.table[i + 2].lexema[0] == 'i')
	//				outFile << "\tpush " << idtable.table[lextable.table[i + 2].idxTI].id << "\n";
	//			else
	//				outFile << "\tpush offset " << idtable.table[lextable.table[i + 2].idxTI].id << "\n";
	//			outFile << "\tcall writes\n";
	//		}
	//		// 
	//		if (lextable.table[i].lexema[0] == 'x')
	//		{
	//			outFile << "\tpush " << idtable.table[lextable.table[i + 2].idxTI].id << "\n";
	//			outFile << "\tcall writed\n";
	//		}
	//		// <
	//		if (lextable.table[i].lexema[0] == 'c')
	//		{
	//			outFile << "mov eax, " << idtable.table[lextable.table[i + 2].idxTI].id << "\n";
	//			outFile << "cmp eax, " << idtable.table[lextable.table[i + 4].idxTI].id << "\n";
	//			outFile << "jb less\n" << "ja more\n";
	//			outFile << "less:\n";
	//		}
	//		// конеу условного блока
	//		if (lextable.table[i].lexema[0] == ']')
	//		{
	//			if (!rif)
	//			{
	//				outFile << "jmp flag\n";
	//				outFile << "more:\n";
	//				rif = true;
	//			}
	//			else
	//			{
	//				outFile << "flag:\n";
	//				rif = false;
	//			}

	//		}
	//		// присваевание
	//		if (lextable.table[i].lexema[0] == '=')
	//		{
	//			indOflex = i - 1;
	//			while (lextable.table[i].lexema[0] != ';')
	//			{
	//				if (lextable.table[i].lexema[0] == 'i')
	//					outFile << "push " << idtable.table[lextable.table[i].idxTI].id << "\n";

	//				if (lextable.table[i].lexema[0] == 'k')
	//					outFile << "push " << idtable.table[lextable.table[i].idxTI].id << "\n";

	//				if (lextable.table[i].lexema[0] == 'l')
	//					outFile << "push offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

	//				if (lextable.table[i].lexema[0] == '@')
	//				{
	//					outFile << "call " << idtable.table[lextable.table[i].idxTI].id << "\n";
	//					outFile << "push eax\n";
	//				}
	//				if (lextable.table[i].lexema[0] == '+')
	//				{
	//					outFile << "pop eax\n";
	//					outFile << "pop ebx\n";
	//					outFile << "add eax, ebx\n";
	//					outFile << "push eax\n";
	//				}
	//				if (lextable.table[i].lexema[0] == '-')
	//				{
	//					outFile << "pop ebx\n";
	//					outFile << "pop eax\n";
	//					outFile << "sub eax, ebx\n";
	//					outFile << "push eax\n";
	//				}
	//				if (lextable.table[i].lexema[0] == '*')
	//				{
	//					outFile << "pop eax\n";
	//					outFile << "pop ebx\n";
	//					outFile << "mul ebx\n";
	//					outFile << "push eax\n";
	//				}
	//				i++;
	//			}
	//			outFile << "pop " << idtable.table[lextable.table[indOflex].idxTI].id << "\n\n";
	//		}
	//	}
	//	outFile << "end main";
	//}
}
