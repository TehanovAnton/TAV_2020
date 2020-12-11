#include "AssemblerGgeneration.h"

namespace AsmGen
{
	void Gen::GenPut(int& i)
	{
		IT::IDDATATYPE outLexDatatype = idtable.table[lextable.table[++i].idxTI].iddatatype;
		if (outLexDatatype == IT::IDDATATYPE::BOOL || outLexDatatype == IT::IDDATATYPE::INT)
		{
			std::cout << "\tpush " << idtable.table[lextable.table[i].idxTI].id << '\n';
			std::cout << "\tpush offset result" << '\n';
			std::cout << "\tcall int_to_char\n";

			std::cout << "\tpush offset ConsleTitle\n";
			std::cout << "\tpush offset result" << '\n';
			std::cout << "\tcall printconsole\n";
		}
		if (outLexDatatype == IT::IDDATATYPE::STR)
		{
			std::cout << "\tpush offset ConsleTitle\n";
			std::cout << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << '\n';
			std::cout << "\tcall printconsole\n";
		}
	}

	/*void Gen::FindElse(int& i)
	{
		int ifBlock = i;
		bool isElse = false;
		for (; lextable.table[i].lexema[0] != LEX_END; i++)
		{
			 if (lextable.table[i].lexema[0] != LEX_ELSE)
		}
	}*/

	void Gen::GenBoolExpresion_RIF_IF(int& i)
	{
		i++;
		for (; lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema[0] == 'i')			   // добавление в стек идентификаторов
				std::cout << "\tpush " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
				std::cout << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].oper_v == '>')	    // >
			{
				std::cout << "cmp eax,ebx\n";
				std::cout << "\tja is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
				std::cout << "\tjmp els" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
			}
			if (lextable.table[i].oper_v == '<')	    // <
			{
				std::cout << "cmp eax,ebx\n";
				std::cout << "\tjb is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
				std::cout << "\tjmp els" << lextable.table[i].sn << lextable.table[i].cn << "\n";
				std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";				
			}
			if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
			{
				std::cout << "cmp eax,ebx\n";
				std::cout << "\tjae is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
				std::cout << "\tjmp els" << lextable.table[i].sn << lextable.table[i].cn << "\n";
				std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
			}
			if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
			{
				std::cout << "cmp eax,ebx\n";
				std::cout << "\tjbe is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
				std::cout << "\tjmp els" << lextable.table[i].sn << lextable.table[i].cn << "\n";
				std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";				
			}
			if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
			{
				std::cout << "\tcmp eax,ebx\n";
				std::cout << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
				std::cout << "\tjmp els\n";
				std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
			}
		}
	}

	void Gen::BodyOfRIF_If(int& i)
	{
		int indOflex = -1;
		int ifLexInd = i;

		GenBoolExpresion_RIF_IF(i);

		for (; lextable.table[i].lexema[0] != LEX_END; i++)
		{
			// присваевание
			if (lextable.table[i].lexema[0] == '=')
			{
				indOflex = i - 1;										// индекс переменной котрой присваивают			
				i++;
				for (; lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
				{
					if (lextable.table[i].lexema[0] == 'i')			   // добавление в стек идентификаторов
						std::cout << "\tpush " << idtable.table[lextable.table[i].idxTI].id << "\n";

					if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
						std::cout << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

					if (isSymInStr(lextable.table[i].lexema[0], JUSTNUBERS))					 // вызов функции
					{
						std::cout << "\tcall " << idtable.table[lextable.table[++i].idxTI].id << "\n";
						std::cout << "\tpush eax\n";												 // резултата в стек
					}

					if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
					{
						std::cout << "\tpop ebx\n";
						std::cout << "\tpop eax\n";

						if (lextable.table[i].oper_v == '+')		  // достать из стека, сложить
							std::cout << "\tadd eax, ebx\n";

						if (lextable.table[i].oper_v == '-')				// достать из стека, отнять
							std::cout << "\tsub eax, ebx\n";

						if (lextable.table[i].oper_v == '*')	    // достать из стека, перемножить
							std::cout << "\tmul ebx\n";

						if (lextable.table[i].oper_v == '/')	    // достать из стека, перемножить
							std::cout << "\tcdq\n\tdiv ebx\n";

						if (lextable.table[i].oper_v == '>')	    // >
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tja is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == '<')	    // <
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjb is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjae is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjbe is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						std::cout << "\tpush eax\n";
					}
				}
				std::cout << "\tpop " << idtable.table[lextable.table[indOflex].idxTI].id << "\n\n"; // присвоение значения
			}

			if (lextable.table[i].lexema[0] == LEX_PUT)
				GenPut(i);

			if (lextable.table[i].lexema[0] == LEX_IF || lextable.table[i].lexema[0] == LEX_RIF)
				BodyOfRIF_If(i);
		}
		std::cout << "els" << lextable.table[ifLexInd].sn << lextable.table[ifLexInd].cn << ":\n";
	}

	void Gen::BodyOfStructures(char* funcProcName, int& indOflex, int& i)
	{
		for (; lextable.table[i].lexema[0] != LEX_END; i++)
		{
			// присваевание
			if (lextable.table[i].lexema[0] == '=')
			{
				indOflex = i - 1;										// индекс переменной котрой присваивают			
				i++;
				for (; lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
				{
					if (lextable.table[i].lexema[0] == 'i')			   // добавление в стек идентификаторов
						std::cout << "\tpush " << idtable.table[lextable.table[i].idxTI].id << "\n";

					if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
						std::cout << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

					if (isSymInStr(lextable.table[i].lexema[0], JUSTNUBERS))					 // вызов функции
					{
						std::cout << "\tcall " << idtable.table[lextable.table[++i].idxTI].id << "\n";
						std::cout << "\tpush eax\n";												 // резултата в стек
					}
					
					if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
					{
						std::cout << "\tpop ebx\n";
						std::cout << "\tpop eax\n";

						if (lextable.table[i].oper_v == '+')		  // достать из стека, сложить
							std::cout << "\tadd eax, ebx\n";

						if (lextable.table[i].oper_v == '-')				// достать из стека, отнять
							std::cout << "\tsub eax, ebx\n";

						if (lextable.table[i].oper_v == '*')	    // достать из стека, перемножить
							std::cout << "\tmul ebx\n";

						if (lextable.table[i].oper_v == '/')	    // достать из стека, перемножить
							std::cout << "\tcdq\n\tdiv ebx\n";

						if (lextable.table[i].oper_v == '>')	    // >
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tja is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == '<')	    // <
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjb is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjae is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjbe is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}						
						std::cout << "\tpush eax\n";
					}
				}
				std::cout << "\tpop " << idtable.table[lextable.table[indOflex].idxTI].id << "\n\n"; // присвоение значения
			}

			if (lextable.table[i].lexema[0] == LEX_PUT)
				GenPut(i);

			if (lextable.table[i].lexema[0] == LEX_IF || lextable.table[i].lexema[0] == LEX_RIF)
				BodyOfRIF_If(i);

			if (lextable.table[i].lexema[0] == LEX_RET)
			{
				std::cout << "\tmov eax," << idtable.table[lextable.table[++i].idxTI].id << '\n';
				std::cout << "\tret" << '\n';
			}
		}
		std::cout << funcProcName << " ENDP\n\n";
	}

	Gen::Gen(LT::LexTable plextable, IT::IdTable pidTable, const char* pathOutFile)
	{
		lextable = plextable;
		idtable = pidTable;


		outFile = std::fstream(pathOutFile, std::ios::out | std::ios::app);
		if (!outFile.is_open())
			throw ERROR_THROW_IN(105, -1, -1);		
		AssemblerHead();
		AssemblerConst();
		AssemblerData();
		AssemblerCode();
		outFile.close();
	}

	void Gen::AssemblerHead()
	{
		std::cout << ".586\n";
		std::cout << "\t.model flat, stdcall\n";
		std::cout << "\tincludelib libucrt.lib\n";
		std::cout << "\tincludelib libucrt.lib\n";
		std::cout << "\tincludelib kernel32.lib\n";
		std::cout << "\tExitProcess PROTO :DWORD\n\n";
		std::cout << "\tincludelib C:\\Users\\Anton\\source\\repos\\programming languages\\TAV_2020\\Debug\\TAV_2020_standartLibary.lib\n";

		std::cout << "\tstrlength PROTO :DWORD\n";		// заголовки функций из станлартонй библиотеки
		std::cout << "\tsubstr PROTO :DWORD\n";
		std::cout << "\tstrl PROTO :DWORD\n";
		std::cout << "\tstrcompare PROTO :DWORD, :DWORD\n";

		std::cout << "\n.stack 4096\n";
	}
	void Gen::AssemblerConst()
	{																
		std::cout << ".const\n";
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::L)
			{
				std::cout << "\t" << idtable.table[i].id;
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR)
					std::cout << " BYTE '" << idtable.table[i].values.vste->str << "', 0";
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
					std::cout << " DWORD " << idtable.table[i].values.vint;
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
					std::cout << " BYTE " << (int)idtable.table[i].values.vbool;
				std::cout << '\n';
			}
		}
	}
	void Gen::AssemblerData()
	{
		std::cout << ".data\n";
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::V)
			{
				
				std::cout << "\t" << idtable.table[i].id << idtable.table[i].vsbAr.aB << idtable.table[i].vsbAr.aB; // имя переменной плюс область видимости
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR || idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
					{ std::cout << " DWORD ?\n"; continue; }
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
				{ std::cout << " BYTE ?\n"; continue; }
				std::cout << '\n';
			}
		}
		std::cout << "ConsoleTitle BYTE \"Console\",0\n";
		std::cout << "result BYTE 100 dup(0)\n";
	}
	void Gen::AssemblerCode()
	{
		std::cout << "\n.code\n";
		int
			indOfProc = 0, indOflex = -1,	 // idx начала func,	idx kex
			rhesis = -1, rif = false,
			proc = false, head = false;		 // func flag,	

		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_DEFF)				// генерация заголовков пользовательских функций
			{
				//i += 2;										// инлекс имени ф.
				indOfProc = i += 2;
				std::cout << idtable.table[lextable.table[i].idxTI].id << " PROC ";
				proc = true;											// начало ген ф.
				for (; lextable.table[i].lexema[0] != LEX_RIGHTHESIS; i++)
				{
					if (lextable.table[i].lexema[0] == 't')
					{
						++i;
						// выставление типа и имени параметра
						if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT ||
							idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
						{
							std::cout << idtable.table[lextable.table[i].idxTI].id << ":DWORD";
						}
						if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
						{
							std::cout << idtable.table[lextable.table[i].idxTI].id << ":BYTE";
						}
						if (lextable.table[i + 1].lexema[0] == LEX_COMMA)
							std::cout << LEX_COMMA << ' ';
						else if (lextable.table[i + 1].lexema[0] == LEX_RIGHTHESIS)
							std::cout << '\n';
					}
				}
				BodyOfStructures(idtable.table[lextable.table[indOfProc].idxTI].id, indOfProc, i);
			}
			continue;

			// старт main proc
			if (lextable.table[i].lexema[0] == 'h')
			{
				head = true;
				std::cout << "main PROC\n";
			}

			// завершение тела func
			if (lextable.table[i].lexema[0] == LEX_END)
			{
				if (proc)
				{
					std::cout << idtable.table[lextable.table[indOfProc].idxTI].id << " ENDP\n\n";
					proc = false;
				}
				else
					std::cout << "call ExitProcess\nmain ENDP\n";
				indOfProc = 0;
			}
			// ret обработка
			if (lextable.table[i].lexema[0] == 'r')
			{
				if (head)
					std::cout << "push 0\n";
				else
				{
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::INT)
						std::cout << "\tmov eax, " << idtable.table[lextable.table[i + 1].idxTI].id << "\n\tret\n";
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
						std::cout << "\tmov eax, offset " << idtable.table[lextable.table[i + 1].idxTI].id << "\n\tret\n";
				}
			}
		//	// вывод в консоль
		//	if (lextable.table[i].lexema[0] == 'y')
		//	{
		//		if (lextable.table[i + 2].lexema[0] == 'i')
		//			std::cout << "\tpush " << idtable.table[lextable.table[i + 2].idxTI].id << "\n";
		//		else
		//			std::cout << "\tpush offset " << idtable.table[lextable.table[i + 2].idxTI].id << "\n";
		//		std::cout << "\tcall writes\n";
		//	}
		//	// 
		//	if (lextable.table[i].lexema[0] == 'x')
		//	{
		//		std::cout << "\tpush " << idtable.table[lextable.table[i + 2].idxTI].id << "\n";
		//		std::cout << "\tcall writed\n";
		//	}
		//	// <
		//	if (lextable.table[i].lexema[0] == 'c')
		//	{
		//		std::cout << "mov eax, " << idtable.table[lextable.table[i + 2].idxTI].id << "\n";
		//		std::cout << "cmp eax, " << idtable.table[lextable.table[i + 4].idxTI].id << "\n";
		//		std::cout << "jb less\n" << "ja more\n";
		//		std::cout << "less:\n";
		//	}
		//	// конеу условного блока
		//	if (lextable.table[i].lexema[0] == ']')
		//	{
		//		if (!rif)
		//		{
		//			std::cout << "jmp flag\n";
		//			std::cout << "more:\n";
		//			rif = true;
		//		}
		//		else
		//		{
		//			std::cout << "flag:\n";
		//			rif = false;
		//		}

		//	}
		std::cout << "end main";
		}
	}
}
