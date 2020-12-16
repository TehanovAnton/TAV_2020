#include "Semantics.h"

namespace SMTCS
{
	OperatioWithTypes::OperatioWithTypes(IT::IDDATATYPE pretType, char oper, int pcountTypes, IT::IDDATATYPE types, ...)
	{
		operation = oper;
		countTypes = pcountTypes;
		retType = pretType;
		IT::IDDATATYPE* p = &types;
		for (int i = 0; i < countTypes && i < MAXTYPENUMBER; i++)
		{
			parmTypes[i] = p[i];
		}
	}

	bool Semantics::start()
	{
		return false;
	}

	// проверка на раваенство операндов по типу данных в выражениях
	bool Semantics::Check_I_DataTypes(int strtIndx)
	{
		IT::IDDATATYPE iddatatype = idTable.table[lexTable.table[strtIndx + 1].idxTI].iddatatype;
		IT::IDTYPE idtype = idTable.table[lexTable.table[strtIndx + 1].idxTI].idtype;

		for (int i = ++strtIndx; lexTable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (lexTable.table[i].lexema[0] == LEX_LITERAL || lexTable.table[i].lexema[0] == LEX_ID)
			{		
				if (iddatatype == idTable.table[lexTable.table[i].idxTI].iddatatype)
					continue;
				else
					throw ERROR_THROW_IN(120, lexTable.table[i].sn, lexTable.table[i].cn);
			}
		}

		return true;
	}

	// проверка на раваенство операндов по типу данных переданному типу
	bool Semantics::Check_I_DataTypes(IT::IDDATATYPE type, char seprtr, int strtIndx)
	{
		for (int i = ++strtIndx; lexTable.table[i].lexema[0] != seprtr; i++)
		{
			if (lexTable.table[i].lexema[0] == LEX_LITERAL || lexTable.table[i].lexema[0] == LEX_ID)
			{
				if (type == idTable.table[lexTable.table[i].idxTI].iddatatype)
					continue;
				else
					throw ERROR_THROW_IN(120, lexTable.table[i].sn, lexTable.table[i].cn);
			}
		}

		return true;
	}

	// поддерживает ли опреация тип
	bool Semantics::IsTyperInOper(OperatioWithTypes operTypes[], IT::IDDATATYPE iddatatype, char oper)
	{
		// цикл по опреациям
		for (int i = 0; i < OPERNUM; i++)
		{
			if (oper == operTypes[i].operation)

				// цикл по типам операции
				for (int e = 0; e < operTypes[i].countTypes; e++)
				{
					if (iddatatype == operTypes[i].parmTypes[e])
						return true;
				}
		}

		return false;
	}

	IT::IDDATATYPE Semantics::Get_O_retType(char oper)
	{
		// цикл по опреациям
		for (int i = 0; i < OPERNUM; i++)
		{
			if (oper == operTypes[i].operation)
				return operTypes[i].retType;
		}

		throw ERROR_THROW_IN(700, -1, -1);
	}

	// проверка на раваенство операторов по типу данных
	bool Semantics::Check_O_DataTypes(char sep,  int strtIndx)
	{
		// тип данных i после равно, (после обработки PL)
		IT::IDDATATYPE operDatatype, iddataType = idTable.table[lexTable.table[strtIndx + 1].idxTI].iddatatype;
		bool firstOper = false;
		for (int i = ++strtIndx; lexTable.table[i].lexema[0] != sep; i++)
		{
			if (LT::IsOperation(lexTable.table[i].lexema[0]))
			{
				// первая операция в строке, возвращаемый тип
				if (!firstOper)
				{
					operDatatype = Get_O_retType(lexTable.table[i].oper_v), firstOper = true;
				}

				// сравненние с последующими оперпциями
				if (operDatatype == Get_O_retType(lexTable.table[i].oper_v) && IsTyperInOper(operTypes, iddataType, lexTable.table[i].oper_v))
					continue;
				else
					throw ERROR_THROW_IN(121, lexTable.table[i].sn, lexTable.table[i].cn);
			}

		}
		return true;
	}	   
		
	bool Semantics::Check_Assigment_DataTypes(IT::IDDATATYPE type, int indx)
	{		   
		// dataType переменной дожен быть равен типу присваемого выражения
		return idTable.table[lexTable.table[indx - 1].idxTI].iddatatype ==
			// dataType присваемого выражения
			type ?
			true : throw ERROR_THROW_IN(122, lexTable.table[indx].sn, lexTable.table[indx].cn);
	}

	void Semantics::CheckOperatoinsSemantics()
	{
		bool res = true;
		for (int i = 0; i < lexTable.posLEX_EQUALSSize; i++)
		{	

			if (lexTable.table[lexTable.posLEX_EQUALS[i]].lexema[0] == LEX_EQUAl)
			{
				// если выражения вызывает функцию то, оно пропускается (проверяется дальше)
				bool func = false;
				for (int e = lexTable.posLEX_EQUALS[i] + 1; lexTable.table[e].lexema[0] != LEX_SEMICOLON; e++)
				{
					if (lexTable.table[e].lexema[0] == LEX_ID)
						if (idTable.table[lexTable.table[e].idxTI].idtype == IT::IDTYPE::F)
						{
							func = true; break;
						}
				}

				if (!func)
				{
					Check_I_DataTypes(lexTable.posLEX_EQUALS[i]);
					Check_O_DataTypes(LEX_SEMICOLON, lexTable.posLEX_EQUALS[i]);
				}
				else
					continue;

				int e = lexTable.posLEX_EQUALS[i];
				// до первой операции или перехода на новую строку
				for (; (lexTable.table[e].lexema[0] != 'v' && lexTable.table[e].lexema[0] != 'w') &&
					lexTable.table[e].lexema[0] != LEX_SEMICOLON;)
					e++;

				// дошли до ; => выражение не имеет операций, вызов функции
				if (lexTable.table[e].lexema[0] == LEX_SEMICOLON)
				{
					// тип данных функции выражений
					IT::IDDATATYPE noOper = idTable.table[lexTable.table[e - 1].idxTI].iddatatype;
					Check_Assigment_DataTypes(noOper, lexTable.posLEX_EQUALS[i]);
				}
				// не дошли до ; => выражение имеет операций, проверка типа слева от равно к типу справа от равно(по возвр типу первой операции)
				else
					Check_Assigment_DataTypes(Get_O_retType(lexTable.table[e].oper_v), lexTable.posLEX_EQUALS[i]);
			}
		}
	}
	///////////////////////////////////////////////////

	bool Semantics::CheckFunctionParms(std::stack<IT::IDDATATYPE> types, int indx)
	{
		bool res = true;
		int i = indx + 1;
		for (; lexTable.table[i].lexema[0] != LEX_RIGHTHESIS; i++)
		{
			if (lexTable.table[i].lexema[0] == 't' && !types.empty())
			{
				res &= idTable.table[lexTable.table[i + 1].idxTI].iddatatype == types.top();
				types.pop();

				if (res)
					continue;
				else
					throw ERROR_THROW_IN(123, lexTable.table[i].sn, lexTable.table[i].cn);
			}
			else if (lexTable.table[i].lexema[0] == 't' && types.empty())
				throw ERROR_THROW_IN(124, lexTable.table[i].sn, lexTable.table[i].cn);

		}
		return types.empty() ? res : throw ERROR_THROW_IN(124, lexTable.table[i].sn, lexTable.table[i].cn);
	}

	void Semantics::CheckFunctionSemantics()
	{		
		IT::Entry idntfctr;
		// перебор LT
		for (int i = 0; i < lexTable.size; i++)
		{			
			// вызываемые функции
			if (lexTable.table[i].lexema[0] == LEX_ID &&
				idTable.table[lexTable.table[i].idxTI].idtype == IT::IDTYPE::F &&
				lexTable.table[i-2].lexema[0] != LEX_DEFF)
			{
				int funcLTidxDef;
				for (int l = 0; l < lexTable.size; l++)
				{
					if (lexTable.table[l].lexema[0] == LEX_ID &&
						idTable.table[lexTable.table[l].idxTI].idtype == IT::IDTYPE::F &&
						lexTable.table[l - 2].lexema[0] == LEX_DEFF)
					{
						if (strcmp(idTable.table[lexTable.table[i].idxTI].id, idTable.table[lexTable.table[l].idxTI].id) == 0)
						{
							funcLTidxDef = l; break;
						}
					}
				}

				// сбор типов фактических параметров; от имени функции до откр скобки
				// m индекс параметров, e количество параметров
				std::stack<IT::IDDATATYPE> types;
				for (int e = atoi(lexTable.table[i - 1].lexema), m = i - 2; e > 0; e--, m--)
				{
					if (lexTable.table[m].lexema[0] == LEX_ID || lexTable.table[m].lexema[0] == LEX_LITERAL)
						types.push(idTable.table[lexTable.table[m].idxTI].iddatatype);
				}

				CheckFunctionParms(types, funcLTidxDef);
			}
		}
	}

	void Semantics::CheckIF_RIF_Semantics()
	{
		bool res = true;
		for (int i = 0; i < lexTable.posLEX_IF_RIF.size(); i++)
		{
			Check_I_DataTypes(IT::IDDATATYPE::BOOL, LEX_RIGHTHESIS, lexTable.posLEX_IF_RIF.front()) &&
			Check_O_DataTypes(LEX_RIGHTHESIS, lexTable.posLEX_IF_RIF.front());
		}
	}

	bool Semantics::CheckIdUsageAtLeasOnce()
	{
		for (int i = 0; i < lexTable.size; i++)
		{
			if (lexTable.table[i].lexema[0] == LEX_ID)
			{

			}
		}

		return false;
	}
}
