#include "AssemblerGgeneration.h"

namespace AsmGen
{
	void Gen::GenPut(int& i)
	{
		IT::IDDATATYPE outLexDatatype = idtable.table[lextable.table[++i].idxTI].iddatatype;
		if (outLexDatatype == IT::IDDATATYPE::BOOL || outLexDatatype == IT::IDDATATYPE::INT)
		{
			outFile << "\tpush " << idtable.table[lextable.table[i].idxTI].id << idtable.table[lextable.table[i].idxTI].vsbAr.aB << idtable.table[lextable.table[i].idxTI].vsbAr.aE << '\n';
			outFile << "\tpush offset result" << '\n';
			outFile << "\tcall int_to_char\n";

			outFile << "\tpush offset ConsoleTitle\n";
			outFile << "\tpush offset result" << '\n';
			outFile << "\tcall printconsole\n";
		}
		if (outLexDatatype == IT::IDDATATYPE::STR)
		{
			outFile << "\tpush offset ConsoleTitle\n";
			if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::V)
			{
				outFile << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << idtable.table[lextable.table[i].idxTI].vsbAr.aB << idtable.table[lextable.table[i].idxTI].vsbAr.aE << '\n';
			}
			else
			{
				outFile << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << '\n';
			}
			outFile << "\tcall printconsole\n";
		}
	}
	void Gen::GenProto(int i)
	{		 
		if (!startPoint)
		{
			int indOfProc = i += 2;                               			// инлекс имени ф.
			outFile << idtable.table[lextable.table[i].idxTI].id << " PROTO ";
		}

		for (; lextable.table[i].lexema[0] != LEX_RIGHTHESIS && lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (startPoint)
				outFile << "main PROC\n", i++;

			else if (lextable.table[i].lexema[0] == 't')
			{
				++i;
				// выставление типа и имени параметра
				if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT ||
					idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
				{
					outFile << ":SDWORD";
				}
				if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
				{
					outFile << idtable.table[lextable.table[i].idxTI].id << ":SBYTE";
				}
				if (lextable.table[i + 1].lexema[0] == LEX_COMMA)
					outFile << LEX_COMMA << ' ';
				else if (lextable.table[i + 1].lexema[0] == LEX_RIGHTHESIS)
					outFile << '\n';
			}
		}
	}
	void Gen::GenBoolExpresion_IF(int& i)
	{
		int elslex = i;
		i++;
		for (; lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema[0] == 'i')			   // добавление в стек идентификаторов
				outFile << "\tpush " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
				outFile << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
			{
				outFile << "\tpop ebx\n";
				outFile << "\tpop eax\n";

				if (lextable.table[i].oper_v == '>')	    // >
				{
					outFile << "cmp eax,ebx\n";
					outFile << "\tja is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp els" << lextable.table[elslex].sn << lextable.table[elslex].cn << "\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == '<')	    // <
				{
					outFile << "cmp eax,ebx\n";
					outFile << "\tjb is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp els" << lextable.table[elslex].sn << lextable.table[elslex].cn << "\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
				{
					outFile << "cmp eax,ebx\n";
					outFile << "\tjae is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp els" << lextable.table[elslex].sn << lextable.table[elslex].cn << "\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
				{
					outFile << "cmp eax,ebx\n";
					outFile << "\tjbe is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp els" << lextable.table[elslex].sn << lextable.table[elslex].cn << "\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
				{
					outFile << "\tcmp eax,ebx\n";
					outFile << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp els" << lextable.table[elslex].sn << lextable.table[elslex].cn << "\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
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
					{
						if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::V)
							outFile << "\tpush " << idtable.table[lextable.table[i].idxTI].id << idtable.table[lextable.table[i].idxTI].vsbAr.aB << idtable.table[lextable.table[i].idxTI].vsbAr.aE << "\n";
						else
							outFile << "\tpush " << idtable.table[lextable.table[i].idxTI].id << '\n';
					}

					if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
						outFile << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

					if (isSymInStr(lextable.table[i].lexema[0], JUSTNUBERS))					 // вызов функции
					{
						outFile << "\tcall " << idtable.table[lextable.table[++i].idxTI].id << "\n";
						outFile << "\tpush eax\n";												 // резултата в стек
					}

					if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
					{
						outFile << "\tpop ebx\n";
						outFile << "\tpop eax\n";
						
						if (lextable.table[i].oper_v == '+')		  // достать из стека, сложить
							outFile << "\tadd eax, ebx\n";

						if (lextable.table[i].oper_v == '-')				// достать из стека, отнять
							outFile << "\tsub eax, ebx\n";

						if (lextable.table[i].oper_v == '*')	    // достать из стека, перемножить
							outFile << "\tcdq\tmul ebx\n";

						if (lextable.table[i].oper_v == '/')	    // достать из стека, перемножить
							outFile << "\tcdq\n\tidiv ebx\n";

						if (lextable.table[i].oper_v == '%')
						{
							outFile << "\tcdq\n\tidiv ebx\n";
							outFile << "mod" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tneg edx\n";
							outFile << "\tcmp edx, 0\n";
							outFile << "jl mod" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, edx\n";
						}

						if (lextable.table[i].oper_v == '>')	    // >
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjg is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == '<')	    // <
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjl is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjge is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjle is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						outFile << "\tpush eax\n";
					}
				}
				outFile << "\tpop " << idtable.table[lextable.table[indOflex].idxTI].id << idtable.table[lextable.table[indOflex].idxTI].vsbAr.aB << idtable.table[lextable.table[indOflex].idxTI].vsbAr.aE << "\n\n"; // присвоение значения
			}

			if (lextable.table[i].lexema[0] == LEX_PUT)
				GenPut(i);

			if (lextable.table[i].lexema[0] == LEX_IF || lextable.table[i].lexema[0] == LEX_RIF)
				BodyOf_IF(i);
		}

		if (!ELS)
			outFile << "els" << lextable.table[ifLexInd].sn << lextable.table[ifLexInd].cn << ":\n";

		if (lextable.table[i].lexema[0] == LEX_ElS)
		{
			BodyOf_IF(i, true);
			outFile << '\n';
		}
	}

	void Gen::GenBoolExpresion_RIF(int& i)
	{
		i++;
		for (; lextable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema[0] == 'i')			   // добавление в стек идентификаторов
				outFile << "\tpush " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
				outFile << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

			if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
			{										
				outFile << "RIF" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				outFile << "\tpop ebx\n";
				outFile << "\tpop eax\n";

				if (lextable.table[i].oper_v == '>')	    // >
				{
					outFile << "cmp eax,ebx\n";
					outFile << "\tjg is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == '<')	    // <
				{
					outFile << "cmp eax,ebx\n";
					outFile << "\tjl is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
				{
					outFile << "cmp eax,ebx\n";
					outFile << "\tjge is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
				{
					outFile << "cmp eax,ebx\n";
					outFile << "\tjle is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
				}
				if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
				{
					outFile << "\tcmp eax,ebx\n";
					outFile << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
					outFile << "\tjmp continue" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
					outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
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
						outFile << "\tpush " << idtable.table[lextable.table[i].idxTI].id << "\n";

					if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
						outFile << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

					if (isSymInStr(lextable.table[i].lexema[0], JUSTNUBERS))					 // вызов функции
					{
						outFile << "\tcall " << idtable.table[lextable.table[++i].idxTI].id << "\n";
						outFile << "\tpush eax\n";												 // резултата в стек
					}

					if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
					{
						outFile << "\tpop ebx\n";
						outFile << "\tpop eax\n";

						if (lextable.table[i].oper_v == '+')		  // достать из стека, сложить
							outFile << "\tadd eax, ebx\n";

						if (lextable.table[i].oper_v == '-')				// достать из стека, отнять
							outFile << "\tsub eax, ebx\n";

						if (lextable.table[i].oper_v == '*')	    // достать из стека, перемножить
							outFile << "\tmul ebx\n";

						if (lextable.table[i].oper_v == '/')	    // достать из стека, перемножить
							outFile << "\tcdq\n\tdiv ebx\n";

						if (lextable.table[i].oper_v == '%')
						{
							outFile << "\tcdq\n\tidiv ebx\n";
							outFile << "mod" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tneg edx\n";
							outFile << "\tcmp edx, 0\n";
							outFile << "jl mod" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, edx";
						}

						if (lextable.table[i].oper_v == '>')	    // >
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjg is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == '<')	    // <
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjl is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt " << lextable.table[i].sn << lextable.table[i].cn << " \n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjge is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt" << lextable.table[i].sn << lextable.table[i].cn <<"\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjle is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt " << lextable.table[i].sn << lextable.table[i].cn <<"\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						outFile << "\tpush eax\n";
					}
				}
				outFile << "\tpop " << idtable.table[lextable.table[indOflex].idxTI].id << "\n\n"; // присвоение значения
			}

			if (lextable.table[i].lexema[0] == LEX_PUT)
				GenPut(i);

			if (lextable.table[i].lexema[0] == LEX_IF)
				BodyOf_IF(i);

			if (lextable.table[i].lexema[0] == LEX_RIF)
				BodyOf_RIF(i);
		}

		outFile << "jmp RIF" << lextable.table[rifLexInd].sn << lextable.table[rifLexInd].cn << ":\n";
		outFile << "continue" << lextable.table[rifLexInd].sn << lextable.table[rifLexInd].cn << ":\n";
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
					{
						if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::V)
							outFile << "\tpush " << idtable.table[lextable.table[i].idxTI].id << idtable.table[lextable.table[i].idxTI].vsbAr.aB << idtable.table[lextable.table[i].idxTI].vsbAr.aE << "\n";
						else
							outFile << "\tpush " << idtable.table[lextable.table[i].idxTI].id << '\n';
					}

					if (lextable.table[i].lexema[0] == 'l')			   // добавление в стек литералов
						outFile << "\tpush offset " << idtable.table[lextable.table[i].idxTI].id << "\n";

					if (isSymInStr(lextable.table[i].lexema[0], JUSTNUBERS))					 // вызов функции
					{
						outFile << "\tcall " << idtable.table[lextable.table[++i].idxTI].id << "\n";
						outFile << "\tpush eax\n";												 // резултата в стек
					}
					
					if (lextable.table[i].lexema[0] == 'v' || lextable.table[i].lexema[0] == 'w')
					{
						outFile << "\tpop ebx\n";
						outFile << "\tpop eax\n";

						if (lextable.table[i].oper_v == '+')		  // достать из стека, сложить
							outFile << "\tadd eax, ebx\n";

						if (lextable.table[i].oper_v == '-')				// достать из стека, отнять
							outFile << "\tsub eax, ebx\n";

						if (lextable.table[i].oper_v == '*')	    // достать из стека, перемножить
							outFile << "\tmul ebx\n";

						if (lextable.table[i].oper_v == '/')	    // достать из стека, перемножить
							outFile << "\tcdq\n\tdiv ebx\n";

						if (lextable.table[i].oper_v == '%')
						{
							outFile << "\tcdq\n\tidiv ebx\n";
							outFile << "modul" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tneg edx\n";
							outFile << "\tcmp edx, 0\n";
							outFile << "jl modul" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, edx\n";
						}

						if (lextable.table[i].oper_v == '>')	    // >
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjg is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == '<')	    // <
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjl is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_MOREEQ_OPER_V)	    // >=
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjge is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_LESSEQ_OPER_V)	    // <=
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tjle is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}
						if (lextable.table[i].oper_v == LEX_BOOLEAQUl_OPER_V)	    // ==
						{
							outFile << "\tcmp eax,ebx\n";
							outFile << "\tje is" << lextable.table[i].sn << lextable.table[i].cn << "\n";
							outFile << "\tmov eax, 0\n";
							outFile << "\tjmp isnt\n";
							outFile << "is" << lextable.table[i].sn << lextable.table[i].cn << ":\n";;
							outFile << "\tmov eax, 1\n";
							outFile << "isnt" << lextable.table[i].sn << lextable.table[i].cn << ":\n";
						}						
						outFile << "\tpush eax\n";
					}
				}
				outFile << "\tpop " << idtable.table[lextable.table[indOflex].idxTI].id << idtable.table[lextable.table[indOflex].idxTI].vsbAr.aB << idtable.table[lextable.table[indOflex].idxTI].vsbAr.aE << "\n\n"; // присвоение значения
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
					outFile << "\tmov eax," << idtable.table[lextable.table[++i].idxTI].id << '\n';
					outFile << "\tret" << '\n';
				}
				else
				{
					outFile << "\tpush " << idtable.table[lextable.table[++i].idxTI].id << '\n';
					outFile << "\tcall ExitProcess\n";
				}
			}
		}
		
		!startPoint ?
			outFile << funcProcName << " ENDP\n\n" :
			outFile << "main ENDP\n";
	}

	Gen::Gen(LT::LexTable plextable, IT::IdTable pidTable, const char* pathOutFile)
	{
		lextable = plextable;
		idtable = pidTable;
		startPoint = false;

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
		outFile << ".586\n";
		outFile << "\t.model flat, stdcall\n";
		outFile << "\tincludelib libucrt.lib\n";
		outFile << "\tincludelib libucrt.lib\n";
		outFile << "\tincludelib kernel32.lib\n";
		outFile << "\tExitProcess PROTO :SDWORD\n\n";
		outFile << "\tincludelib ..\\Debug\\TAV_2020_standartLibary.lib\n";
		outFile << "\tincludelib ..\\Debug\\asmlib.lib\n\n"; // библиотека asm для print, int_to_char

		outFile << "\tWriteConsoleA PROTO : SDWORD, : SDWORD, : SDWORD, : SDWORD, : SDWORD\n";
		outFile << "\tSetConsoleTitleA PROTO : SDWORD\n";
		outFile << "\tGetStdHandle PROTO : SDWORD\n";
		outFile << "\tprintconsole PROTO : SDWORD, : SDWORD\n";
		outFile << "\tint_to_char PROTO : SDWORD, : SDWORD\n\n";

		outFile << "\tconca PROTO : SDWORD, : SDWORD\n";		// заголовки функций из стандартонй библиотеки
		outFile << "\tcompa PROTO : SDWORD, : SDWORD\n";

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
					outFile << " SBYTE '" << idtable.table[i].values.vste->str << "', 0";
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
					outFile << " SDWORD " << idtable.table[i].values.vint;
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
					outFile << " SBYTE " << (int)idtable.table[i].values.vbool;
				outFile << '\n';
			}
		}
	}
	void Gen::AssemblerData()
	{
		outFile << ".data\n";
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::V)
			{
				
				outFile << "\t" << idtable.table[i].id << idtable.table[i].vsbAr.aB << idtable.table[i].vsbAr.aE; // имя переменной плюс область видимости
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR || idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
					{ outFile << " SDWORD ?\n"; continue; }
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
				{ outFile << " SBYTE ?\n"; continue; }
				outFile << '\n';
			}
		}
		outFile << "\tConsoleTitle SBYTE \"Console\",0\n";
		outFile << "\tresult SBYTE 100 dup(0)\n";
	}
	void Gen::AssemblerCode()
	{
		outFile << "\n.code\n";
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
					outFile << idtable.table[lextable.table[i].idxTI].id << " PROC ";		  // генерация заголовков пользовательских функций
					for (; lextable.table[i].lexema[0] != LEX_RIGHTHESIS; i++)
					{
						if (lextable.table[i].lexema[0] == 't')
						{
							++i;
							// выставление типа и имени параметра
							if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT ||
								idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
							{
								outFile << idtable.table[lextable.table[i].idxTI].id << ":SDWORD";
							}
							if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
							{
								outFile << idtable.table[lextable.table[i].idxTI].id << ":SBYTE";
							}
							if (lextable.table[i + 1].lexema[0] == LEX_COMMA)
								outFile << LEX_COMMA << ' ';
							else if (lextable.table[i + 1].lexema[0] == LEX_RIGHTHESIS)
								outFile << '\n';
						}
					}
				}
				BodyOfFunctions(idtable.table[lextable.table[indOfProc].idxTI].id, indOfProc, i);
			}	
		}

		outFile << "end main";
	}
}
