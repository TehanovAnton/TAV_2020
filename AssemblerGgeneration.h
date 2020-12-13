#pragma once
#include"LT.h"
#include"IT.h"
//#include<iostream>
#include<fstream>

namespace AsmGen
{
	struct Gen
	{
		LT::LexTable lextable;
		IT::IdTable	 idtable;
		std::fstream outFile;
		Gen(LT::LexTable plextable, IT::IdTable pidTable, const char* pathOutFile);

		bool startPoint;

		void AssemblerHead();
		void AssemblerConst();
		void AssemblerData();
		void AssemblerCode();	
		void BodyOfFunctions(char* funcProcName, int& indOflex, int& i);
		void BodyOf_IF(int& i, bool ELS = false);
		void GenBoolExpresion_IF(int& i);
		void BodyOf_RIF(int& i);
		void GenBoolExpresion_RIF(int& i);
		void GenPut(int& i);
		void GenProto(int i);
	};
}