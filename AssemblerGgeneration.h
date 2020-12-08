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
		std::ofstream outFile;

		void AssemblerHead();
		void AssemblerConst();
		void AssemblerData();
		void AssemblerCode();
	};
}