#include "Semantics.h"

namespace SMTCS
{
	// массив операций и поддерживаемых типов
	OperatioWithTypes operTypes[OPERNUM] = { ARITHMETICOPER, BOOLEANOPER };

	OperatioWithTypes::OperatioWithTypes(char oper, int pcountTypes, IT::IDDATATYPE types, ...)
	{
		operation = oper;
		countTypes = pcountTypes;
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

	// проверка на раваенство операндов по типу данных
	bool Check_I_DataTypes(LT::LexTable lexTable, IT::IdTable idTable, int strtIndx)
	{
		IT::IDDATATYPE iddatatype = idTable.table[lexTable.table[strtIndx + 1].idxTI].iddatatype;
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
	}

	// поддерживает ли опреация тип
	bool IsTyperInOper(OperatioWithTypes operTypes[], IT::IDDATATYPE iddatatype, char oper)
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

	// проверка на раваенство операторов по типу данных
	bool Check_O_DataTypes(LT::LexTable lexTable, IT::IdTable idTable, int strtIndx)
	{
		IT::IDDATATYPE iddatatype = idTable.table[lexTable.table[strtIndx + 1].idxTI].iddatatype;
		for (int i = ++strtIndx; lexTable.table[i].lexema[0] != LEX_SEMICOLON; i++)
		{
			if (lexTable.table[i].lexema[0] == 'v' )
			{
				// сравненние
				if (IsTyperInOper(operTypes, iddatatype, lexTable.table[i].oper_v))
					continue;
				else
					throw ERROR_THROW_IN(121, lexTable.table[i].sn, lexTable.table[i].cn);
			}
		}
		return true;
	}	   
		
	bool Check_Assigment_DataTypes(LT::LexTable lexTable, IT::IdTable idTable, int indx)
	{		   
		// dataType переменной дожен быть равен типу присваемого выражения
		return idTable.table[lexTable.table[indx - 1].idxTI].iddatatype ==
			// dataType присваемого выражения
			idTable.table[lexTable.table[indx + 1].idxTI].iddatatype ?
			true : throw ERROR_THROW_IN(122, lexTable.table[indx].sn, lexTable.table[indx].cn);
	}

	bool Semantics::CheckOperatoinsSemantics()
	{
		int i = 0;
		bool res = true;
		for (LT::Entry lex = lexTable.table[i]; i < lexTable.posLEX_EQUALSSize; i++, lex = lexTable.table[i])
		{
			res &=
				Check_O_DataTypes(lexTable, idTable, lexTable.posLEX_EQUALS[i]) &&
				Check_I_DataTypes(lexTable, idTable, lexTable.posLEX_EQUALS[i]) &&
				Check_Assigment_DataTypes(lexTable, idTable, lexTable.posLEX_EQUALS[i]);
		}
		return res;
	}
	///////////////////////////////////////////////////

	bool CheckFunctionParms(std::queue<IT::IDDATATYPE> types, LT::LexTable lexTable, IT::IdTable idTable, int indx)
	{
		bool res = true;
		int i = indx + 1;
		for (; lexTable.table[i].lexema[0] != LEX_RIGHTHESIS; i++)
		{
			if (lexTable.table[i].lexema[0] == 't' && !types.empty())
			{
				res &= idTable.table[lexTable.table[i + 1].idxTI].iddatatype == types.front();
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

	bool Semantics::CheckFunctionSemantics()
	{
		std::queue<IT::IDDATATYPE> types;
		IT::Entry idntfctr;
		// перебор LT
		for (int i = 0; i < lexTable.size; i++)
		{			
			// вызываемые функции
			if (lexTable.table[i].lexema[0] == LEX_ID)
			{
				idntfctr = idTable.table[lexTable.table[i].idxTI];
				int funcLTidxDef = 0;
				if (idntfctr.idtype == IT::IDTYPE::F && idntfctr.idxfirstLE != i)
				{
					funcLTidxDef = idntfctr.idxfirstLE;
					// сбор типов фактических параметров; от имени функции до закрывающей скобки
					for (i++; lexTable.table[i].lexema[0] != LEX_RIGHTHESIS;
						i++)
					{
						if (lexTable.table[i].lexema[0] == LEX_ID || lexTable.table[i].lexema[0] == LEX_LITERAL)
						{
							idntfctr = idTable.table[lexTable.table[i].idxTI];
							types.push(idntfctr.iddatatype);
						}
					}

					return CheckFunctionParms(types, lexTable, idTable, funcLTidxDef);
				}
			}
		}
		return true;
	}
}
