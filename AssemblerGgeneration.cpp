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
	void Gen::GenProto(int i)
	{		 
		if (!startPoint)
		{
			int indOfProc = i += 2;                               			// инлекс имени ф.
			std::cout << idtable.table[lextable.table[i].idxTI].id << " PROTO ";
		}

		for (; lextable.table[i].lexema[0] != LEX_RIGHTHESIS && lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (startPoint)
				std::cout << "main PROC\n";

			else if (lextable.table[i].lexema[0] == 't')
			{
				++i;
				// выставление типа и имени параметра
				if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT ||
					idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
				{
					std::cout << ":SDWORD";
				}
				if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
				{
					std::cout << idtable.table[lextable.table[i].idxTI].id << ":SBYTE";
				}
				if (lextable.table[i + 1].lexema[0] == LEX_COMMA)
					std::cout << LEX_COMMA << ' ';
				else if (lextable.table[i + 1].lexema[0] == LEX_RIGHTHESIS)
					std::cout << '\n';
			}
		}
	}
	void Gen::GenBoolExpresion_IF(int& i)
	{
		i++;
		for (; lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema[0] == 'i')			   // добавление в стек идентификаторов
				std::cout << "\tpush " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
				std::cout << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
			{
				std::cout << "\tpop ebx\n";
				std::cout << "\tpop eax\n";

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
	}
	void Gen::BodyOf_IF(int& i, bool ELS)
	{
		int indOflex = -1;
		int ifLexInd = i;

		GenBoolExpresion_IF(i);

		for (; lextable.table[i].lexema[0] != LEX_END && lextable.table[i].lexema[0] != LEX_ElS; i++)
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
							std::cout << "\tcdq\tmul ebx\n";

						if (lextable.table[i].oper_v == '/')	    // достать из стека, перемножить
							std::cout << "\tcdq\n\tidiv ebx\n";

						if (lextable.table[i].oper_v == '%')
						{
							std::cout << "\tcdq\n\tidiv ebx\n";
							std::cout << "mod" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tneg edx\n";
							std::cout << "\tcmp edx, 0\n";
							std::cout << "jl mod" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, edx\n";
						}

						if (lextable.table[i].oper_v == '>')	    // >
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjg is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == '<')	    // <
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjl is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjge is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjle is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
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
				BodyOf_IF(i);
		}

		if (!ELS)
			std::cout << "els" << lextable.table[ifLexInd].sn << lextable.table[ifLexInd].cn << ":\n";

		if (lextable.table[i].lexema[0] == LEX_ElS)
		{
			BodyOf_IF(i, true);
			std::cout << '\n';
		}
	}

	void Gen::GenBoolExpresion_RIF(int& i)
	{
		i++;
		for (; lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema[0] == 'i')			   // добавление в стек идентификаторов
				std::cout << "\tpush " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
				std::cout << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
			{										
				std::cout << "RIF" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				std::cout << "\tpop ebx\n";
				std::cout << "\tpop eax\n";

				if (lextable.table[i].oper_v == '>')	    // >
				{
					std::cout << "cmp eax,ebx\n";
					std::cout << "\tjg is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					std::cout << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == '<')	    // <
				{
					std::cout << "cmp eax,ebx\n";
					std::cout << "\tjl is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					std::cout << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
				{
					std::cout << "cmp eax,ebx\n";
					std::cout << "\tjge is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					std::cout << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
				{
					std::cout << "cmp eax,ebx\n";
					std::cout << "\tjle is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					std::cout << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
				{
					std::cout << "\tcmp eax,ebx\n";
					std::cout << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					std::cout << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
			}
		}
	}
	void Gen::BodyOf_RIF(int& i)
	{
		int indOflex = -1;
		int rifLexInd = i;

		GenBoolExpresion_RIF(i);

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

						if (lextable.table[i].oper_v == '%')
						{
							std::cout << "\tcdq\n\tidiv ebx\n";
							std::cout << "mod" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tneg edx\n";
							std::cout << "\tcmp edx, 0\n";
							std::cout << "jl mod" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, edx";
						}

						if (lextable.table[i].oper_v == '>')	    // >
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjg is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == '<')	    // <
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjl is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjge is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjle is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
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

			if (lextable.table[i].lexema[0] == LEX_IF)
				BodyOf_IF(i);

			if (lextable.table[i].lexema[0] == LEX_RIF)
				BodyOf_RIF(i);
		}

		std::cout << "jmp RIF" << lextable.table[rifLexInd].sn << lextable.table[rifLexInd].cn << ":\n";
		std::cout << "continue" << lextable.table[rifLexInd].sn << lextable.table[rifLexInd].cn << ":\n";
	}
	void Gen::BodyOfFunctions(char* funcProcName, int& indOflex, int& i)
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

						if (lextable.table[i].oper_v == '%')
						{
							std::cout << "\tcdq\n\tidiv ebx\n";
							std::cout << "modul" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tneg edx\n";
							std::cout << "\tcmp edx, 0\n";
							std::cout << "jl modul" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, edx\n";
						}

						if (lextable.table[i].oper_v == '>')	    // >
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjg is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == '<')	    // <
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjl is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjge is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							std::cout << "\tmov eax, 0\n";
							std::cout << "\tjmp isnt\n";
							std::cout << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							std::cout << "\tmov eax, 1\n";
							std::cout << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
						{
							std::cout << "\tcmp eax,ebx\n";
							std::cout << "\tjle is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
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

			if (lextable.table[i].lexema[0] == LEX_IF)
				BodyOf_IF(i);

			if (lextable.table[i].lexema[0] == LEX_RIF)
				BodyOf_RIF(i);

			if (lextable.table[i].lexema[0] == LEX_RET)
			{
				if (!startPoint)
				{
					std::cout << "\tmov eax," << idtable.table[lextable.table[++i].idxTI].id << '\n';
					std::cout << "\tret" << '\n';
				}
				else
				{
					std::cout << "\tpush " << idtable.table[lextable.table[++i].idxTI].id << '\n';
					std::cout << "\tcall ExitProcess\n";
				}
			}
		}
		
		!startPoint ?
			std::cout << funcProcName << " ENDP\n\n" :
			std::cout << "main ENDP\n";
	}

	Gen::Gen(LT::LexTable plextable, IT::IdTable pidTable, const char* pathOutFile)
	{
		lextable = plextable;
		idtable = pidTable;
		startPoint = false;

		/*outFile = std::fstream(pathOutFile, std::ios::out | std::ios::app);
		if (!outFile.is_open())
			throw ERROR_THROW_IN(105, -1, -1);		*/
		AssemblerHead();
		AssemblerConst();
		AssemblerData();
		AssemblerCode();
		/*outFile.close();*/
	}
	void Gen::AssemblerHead()
	{
		std::cout << ".586\n";
		std::cout << "\t.model flat, stdcall\n";
		std::cout << "\tincludelib libucrt.lib\n";
		std::cout << "\tincludelib libucrt.lib\n";
		std::cout << "\tincludelib kernel32.lib\n";
		std::cout << "\tExitProcess PROTO :SDWORD\n\n";
		std::cout << "\tincludelib C:\\Users\\Anton\\source\\repos\\programming languages\\TAV_2020\\Debug\\TAV_2020_standartLibary.lib\n";
		std::cout << "\tincludelib C:\\Users\\Anton\\source\\repos\\asmlib.lib\n\n"; // библиотека asm для print, int_to_char

		std::cout << "\tWriteConsoleA PROTO : SDWORD, : SDWORD, : SDWORD, : SDWORD, : SDWORD\n";
		std::cout << "\tSetConsoleTitleA PROTO : SDWORD\n";
		std::cout << "\tGetStdHandle PROTO : SDWORD\n";
		std::cout << "\tprintconsole PROTO : SDWORD, : SDWORD\n";
		std::cout << "\tint_to_char PROTO : SDWORD, : SDWORD\n\n";

		std::cout << "\tconcat PROTO :SDWORD\n";		// заголовки функций из стандартонй библиотеки
		std::cout << "\tcompare PROTO :SDWORD\n";

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
					std::cout << " SBYTE '" << idtable.table[i].values.vste->str << "', 0";
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
					std::cout << " SDWORD " << idtable.table[i].values.vint;
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
					std::cout << " SBYTE " << (int)idtable.table[i].values.vbool;
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
				
				std::cout << "\t" << idtable.table[i].id << idtable.table[i].vsbAr.aB << idtable.table[i].vsbAr.aE; // имя переменной плюс область видимости
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR || idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
					{ std::cout << " SDWORD ?\n"; continue; }
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
				{ std::cout << " SBYTE ?\n"; continue; }
				std::cout << '\n';
			}
		}
		std::cout << "\tConsoleTitle SBYTE \"Console\",0\n";
		std::cout << "\tresult SBYTE 100 dup(0)\n";
	}
	void Gen::AssemblerCode()
	{
		std::cout << "\n.code\n";
		int
			indOfProc = 0;	 // idx начала func

		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_DEFF)
			{
				if (lextable.table[i + 1].lexema[0] == LEX_STARTPOINT)
					startPoint = true;

				GenProto(i);															  // генерация прототипов

				if (!startPoint)
				{
					indOfProc = i += 2;						// инлекс имени ф.
					std::cout << idtable.table[lextable.table[i].idxTI].id << " PROC ";		  // генерация заголовков пользовательских функций
					for (; lextable.table[i].lexema[0] != LEX_RIGHTHESIS; i++)
					{
						if (lextable.table[i].lexema[0] == 't')
						{
							++i;
							// выставление типа и имени параметра
							if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT ||
								idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
							{
								std::cout << idtable.table[lextable.table[i].idxTI].id << ":SDWORD";
							}
							if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
							{
								std::cout << idtable.table[lextable.table[i].idxTI].id << ":SBYTE";
							}
							if (lextable.table[i + 1].lexema[0] == LEX_COMMA)
								std::cout << LEX_COMMA << ' ';
							else if (lextable.table[i + 1].lexema[0] == LEX_RIGHTHESIS)
								std::cout << '\n';
						}
					}
				}
				BodyOfFunctions(idtable.table[lextable.table[indOfProc].idxTI].id, indOfProc, i);
			}	
		}

		std::cout << "end main";
	}
}
