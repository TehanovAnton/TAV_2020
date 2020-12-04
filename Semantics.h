#pragma once
#include"LT.h"
#include"IT.h"
#include<queue>

#define MAXTYPENUMBER 3
#define OPERNUM 9
#define ARITHMETICOPER  {'+', 2, IT::IDDATATYPE::INT, IT::IDDATATYPE::STR}, \
						{'-', 2, IT::IDDATATYPE::INT, IT::IDDATATYPE::STR}, \
						{'*', 1, IT::IDDATATYPE::INT}, { '/', 1, IT::IDDATATYPE::INT } \

#define BOOLEANOPER  {'>', 1, IT::IDDATATYPE::INT}, {'<', 1, IT::IDDATATYPE::INT},	\
					{LEX_MOREEQ_OPER_V, 1, IT::IDDATATYPE::INT}, { LEX_LESSEQ_OPER_V, 1, IT::IDDATATYPE::INT }, \
					{ LEX_BOOLEAQUl_OPER_V, 3, IT::IDDATATYPE::INT, IT::IDDATATYPE::BOOL, IT::IDDATATYPE::STR }	\

namespace SMTCS
{
	struct OperatioWithTypes
	{
		char operation;
		IT::IDDATATYPE parmTypes[MAXTYPENUMBER];
		int countTypes;

		OperatioWithTypes(char oper, int pcountTypes, IT::IDDATATYPE types, ...);
	};

	struct Semantics
	{
		LT::LexTable lexTable;
		IT::IdTable idTable;		

		Semantics(LT::LexTable plexTbale, IT::IdTable pidTable)
		{
			lexTable = plexTbale;
			idTable = pidTable;
		}

		bool start();
		bool CheckOperatoinsSemantics();
		bool CheckFunctionSemantics();
	};
}