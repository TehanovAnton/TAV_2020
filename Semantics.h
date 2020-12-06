#pragma once
#include"LT.h"
#include"IT.h"
#include<queue>

#define MAXTYPENUMBER 3
#define OPERNUM 9
#define ARITHMETICOPER  {IT::IDDATATYPE::INT, '+', 2, IT::IDDATATYPE::INT, IT::IDDATATYPE::STR}, \
						{IT::IDDATATYPE::INT, '-', 2, IT::IDDATATYPE::INT, IT::IDDATATYPE::STR}, \
						{IT::IDDATATYPE::INT, '*', 1, IT::IDDATATYPE::INT}, {IT::IDDATATYPE::INT, '/', 1, IT::IDDATATYPE::INT } \

#define BOOLEANOPER  {IT::IDDATATYPE::BOOL , '>', 1, IT::IDDATATYPE::INT}, {IT::IDDATATYPE::BOOL, '<', 1, IT::IDDATATYPE::INT},	\
					{IT::IDDATATYPE::INT, LEX_MOREEQ_OPER_V, 1, IT::IDDATATYPE::INT}, { IT::IDDATATYPE::BOOL, LEX_LESSEQ_OPER_V, 1, IT::IDDATATYPE::INT }, \
					{ IT::IDDATATYPE::BOOL, LEX_BOOLEAQUl_OPER_V, 3, IT::IDDATATYPE::INT, IT::IDDATATYPE::BOOL, IT::IDDATATYPE::STR }	\

namespace SMTCS
{
	struct OperatioWithTypes
	{
		char operation;
		IT::IDDATATYPE parmTypes[MAXTYPENUMBER];
		IT::IDDATATYPE retType;
		int countTypes;

		OperatioWithTypes(IT::IDDATATYPE pretType, char oper, int pcountTypes, IT::IDDATATYPE types, ...);
	};

	struct Semantics
	{
		LT::LexTable lexTable;
		IT::IdTable idTable;

		// массив операций и поддерживаемых типов
		OperatioWithTypes operTypes[OPERNUM] = { ARITHMETICOPER, BOOLEANOPER };

		Semantics(LT::LexTable plexTbale, IT::IdTable pidTable)
		{
			lexTable = plexTbale;
			idTable = pidTable;
		}

		bool start();
		void CheckOperatoinsSemantics();
		void CheckFunctionSemantics();
		void CheckIF_RIF_Semantics();
		bool CheckIdUsageAtLeasOnce();

		bool CheckFunctionParms(std::queue<IT::IDDATATYPE> types, int indx);
		bool Check_Assigment_DataTypes(IT::IDDATATYPE type, int indx);
		bool Check_O_DataTypes(char sep, int strtIndx);
		IT::IDDATATYPE Get_O_retType(char oper);
		bool IsTyperInOper(OperatioWithTypes operTypes[], IT::IDDATATYPE iddatatype, char oper);
		bool Check_I_DataTypes(IT::IDDATATYPE type, char seprtr, int strtIndx);
		bool Check_I_DataTypes(int strtIndx);
	};
}