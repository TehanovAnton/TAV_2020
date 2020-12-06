#pragma once
#include<stack>
#include"LT.h"
#include"IT.h"
#include"SpecialSym.h"

#define test0 "=i-l*i;"								 
#define test1 "=(l+l)*(l+l)-i;"
#define test2 "=i*i(i+i(i(i,i)+i,i))-i/i(i,i);"
#define test3 "=i*[i+[[i,i]+i,i]]-i/[i,i];"
#define test4 "=i*i()-i/i(i,i);"

#define SymWithPriorytyAmount 13
#define Operations_Brackets "+-*/<>ghc()[]"
#define Operations "+-*/<>ghc"
#define prior_1 {'(', 1}, {')', 1}, {'[', 1}, {']', 1}
#define prior_2 {'+', 2}, {'-', 2}, {LEX_BOOLEAQUl_OPER_V, 2}, {LEX_LESSEQ_OPER_V, 2}, {LEX_MOREEQ_OPER_V, 2}, {'>', 2}, {'<', 2}
#define prior_3 {'*', 3}, {'/', 3}
									 
namespace PN // PolishNotation
{
	struct PNstr
	{
		int maxsize;
		int size;
		char* pnstr;
		PNstr();
		PNstr(int m, int s, char* pns);
	};

	struct PNLTstr
	{
		int maxsize;
		int size;
		int* origIdx;
		LT::Entry* pnstr;
		PNLTstr();
		PNLTstr(int m);
		void Addpnstr(LT::Entry entryL, IT::IdTable& idTable, int indx, int origIdx);
	};

	struct SymWithPrioryty
	{
		char sym;
		short pRioryty;

		SymWithPrioryty();
		SymWithPrioryty(char s, short p);		
	};

	int GetPrioryty(char sym, SymWithPrioryty prior[]);

	void parseFunctoPnstrLT(LT::LexTable& lexTable, IT::IdTable& idTable, PNLTstr& res, int& i, std::stack<LT::Entry>& stck, SymWithPrioryty prioryties[]);
	void parseArrtoPnstrLT(LT::LexTable& lexTable, IT::IdTable& idTable, PNLTstr& res, int& i, std::stack<LT::Entry>& stck, SymWithPrioryty prioryties[]);
	bool PolishNotation(int exprStart, LT::LexTable& lexTable, IT::IdTable& idTable, int& countEmptyEl);
}