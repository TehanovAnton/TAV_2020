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
		IT::IDDATATYPE operDatatype, iddataType = idTable.table[lexTable.table[strtIndx + 1].idxTI].iddatatype;
		bool firstOper = false;
		for (int i = ++strtIndx; lexTable.table[i].lexema[0] != sep; i++)
		{
			if (LT::IsOperation(lexTable.table[i].lexema[0]))
			{
				// поиск первой операции
				if (!firstOper)
					operDatatype = Get_O_retType(lexTable.table[i].oper_v), firstOper = true;

				// сравненние
				if (firstOper)
				{
					if (firstOper && operDatatype == Get_O_retType(lexTable.table[i].oper_v) && IsTyperInOper(operTypes, iddataType, lexTable.table[i].oper_v))
						continue;
					else
						throw ERROR_THROW_IN(121, lexTable.table[i].sn, lexTable.table[i].cn);
				}
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
			Check_O_DataTypes(LEX_SEMICOLON, lexTable.posLEX_EQUALS[i]);
			Check_I_DataTypes(lexTable.posLEX_EQUALS[i]);

			int e = lexTable.posLEX_EQUALS[i];
			IT::IDDATATYPE noOper = idTable.table[lexTable.table[e + 1].idxTI].iddatatype;				  // тип для выражений из одного операнда
			for (; (lexTable.table[e].lexema[0] != 'v' && lexTable.table[e].lexema[0] != 'w') &&
				lexTable.table[e].lexema[0] != LEX_SEMICOLON;)	  // до первой операции
				e++;

			if (lexTable.table[e].lexema[0] == LEX_SEMICOLON)				// выражение не имеет операций
				Check_Assigment_DataTypes(noOper, lexTable.posLEX_EQUALS[i]);
			else
				Check_Assigment_DataTypes(Get_O_retType(lexTable.table[e].oper_v), lexTable.posLEX_EQUALS[i]);
		}
	}
	///////////////////////////////////////////////////

	bool Semantics::CheckFunctionParms(std::queue<IT::IDDATATYPE> types, int indx)
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

	void Semantics::CheckFunctionSemantics()
	{
		std::queue<IT::IDDATATYPE> types;
		IT::Entry idntfctr;
		// перебор LT
		for (int i = 0; i < lexTable.size; i++)
		{			
			// вызываемые функции
			if (lexTable.table[i].lexema[0] == LEX_ID &&
				idTable.table[lexTable.table[i].idxTI].idtype == IT::IDTYPE::F &&
				idTable.table[lexTable.table[i].idxTI].idxfirstLE != i)
			{
				idntfctr = idTable.table[lexTable.table[i].idxTI];
				int funcLTidxDef = idntfctr.idxfirstLE;
				// сбор типов фактических параметров; от имени функции до закрывающей скобки
				for (int e = atoi(lexTable.table[i - 1].lexema), m = i - 2; e > 0; e--, m--)
				{
					if (lexTable.table[m].lexema[0] == LEX_ID || lexTable.table[m].lexema[0] == LEX_LITERAL)
					{
						idntfctr = idTable.table[lexTable.table[m].idxTI];
						types.push(idntfctr.iddatatype);
					}
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
