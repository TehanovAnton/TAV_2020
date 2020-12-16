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

	// �������� �� ���������� ��������� �� ���� ������ � ����������
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

	// �������� �� ���������� ��������� �� ���� ������ ����������� ����
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

	// ������������ �� �������� ���
	bool Semantics::IsTyperInOper(OperatioWithTypes operTypes[], IT::IDDATATYPE iddatatype, char oper)
	{
		// ���� �� ���������
		for (int i = 0; i < OPERNUM; i++)
		{
			if (oper == operTypes[i].operation)

				// ���� �� ����� ��������
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
		// ���� �� ���������
		for (int i = 0; i < OPERNUM; i++)
		{
			if (oper == operTypes[i].operation)
				return operTypes[i].retType;
		}

		throw ERROR_THROW_IN(700, -1, -1);
	}

	// �������� �� ���������� ���������� �� ���� ������
	bool Semantics::Check_O_DataTypes(char sep,  int strtIndx)
	{
		// ��� ������ i ����� �����, (����� ��������� PL)
		IT::IDDATATYPE operDatatype, iddataType = idTable.table[lexTable.table[strtIndx + 1].idxTI].iddatatype;
		bool firstOper = false;
		for (int i = ++strtIndx; lexTable.table[i].lexema[0] != sep; i++)
		{
			if (LT::IsOperation(lexTable.table[i].lexema[0]))
			{
				// ������ �������� � ������, ������������ ���
				if (!firstOper)
				{
					operDatatype = Get_O_retType(lexTable.table[i].oper_v), firstOper = true;
				}

				// ���������� � ������������ ����������
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
		// dataType ���������� ����� ���� ����� ���� ����������� ���������
		return idTable.table[lexTable.table[indx - 1].idxTI].iddatatype ==
			// dataType ����������� ���������
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
				// ���� ��������� �������� ������� ��, ��� ������������ (����������� ������)
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
				// �� ������ �������� ��� �������� �� ����� ������
				for (; (lexTable.table[e].lexema[0] != 'v' && lexTable.table[e].lexema[0] != 'w') &&
					lexTable.table[e].lexema[0] != LEX_SEMICOLON;)
					e++;

				// ����� �� ; => ��������� �� ����� ��������, ����� �������
				if (lexTable.table[e].lexema[0] == LEX_SEMICOLON)
				{
					// ��� ������ ������� ���������
					IT::IDDATATYPE noOper = idTable.table[lexTable.table[e - 1].idxTI].iddatatype;
					Check_Assigment_DataTypes(noOper, lexTable.posLEX_EQUALS[i]);
				}
				// �� ����� �� ; => ��������� ����� ��������, �������� ���� ����� �� ����� � ���� ������ �� �����(�� ����� ���� ������ ��������)
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
		// ������� LT
		for (int i = 0; i < lexTable.size; i++)
		{			
			// ���������� �������
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

				// ���� ����� ����������� ����������; �� ����� ������� �� ���� ������
				// m ������ ����������, e ���������� ����������
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
