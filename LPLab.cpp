#include"LPLab.h"
#include "PolishNotation.h"
#include"AssemblerGgeneration.h"
#include"MFST.h"
#include"Semantics.h"

#define TAVasmOutFile "..\\TAV_2020\\TAVasm\\TAVams.asm"

#define LPLab 1	
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		std::wcout << "-in:" << parm.in << ", -out:" << parm.out << ", log:" << parm.log << std::endl << std::endl;

		log = Log::getlog(parm.log);

		In::IN in = In::getin(parm.in);
		std::cout << in.text << "\n\n";
		std::cout << "всего символов: " << in.size << std::endl;
		std::cout << "всего строк: " << in.lines << std::endl;
		std::cout << "пропущенно: " << in.ignor << "\n\n";

		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);

		std::fstream file(parm.in, std::ios::in | std::ios::out);

		// считал в переменную текст
		std::string str = "";
		for (char ch; file.get(ch); ) { str += ch; }
		file.close();

		// раставлени сепараторов, очистка от повторяющихся переносов и пробелов
		// LPLab14-1
		std::string resStr = processText(str);

		LT::LexTable lexTable = LT::Create(LT_MAXSIZE);
		lexTable.GetLexemsPosition(str);					// заполнение массива положений

		// запись в parm.out
		file.open(parm.out, std::ios::out);
		file << resStr;
		str.clear();
		file.close();

		IT::IdTable idTable = IT::Create(TI_MAXSIZE);

		// LPLab14-2
		LTITBuilding(lexTable, idTable, resStr);

		//вывод LT
		lexTable.PrintTable();
		std::cout << "\n\n\n";

		//вывод IT
		idTable.PrintTable();


		////LPLab16
		//MFST_TRACE_START											   // отладка
		//MFST::Mfst mfst(lexTable, GRB::getGreibach());			   // автомат
		//if (!mfst.start())												   // старт синтаксического анализа
		//	throw ERROR_THROW_IN(605, -1, -1)
		//mfst.printrules();

		////LPLab15
		exposingNamespaces(lexTable, idTable);
		ChangeLTWithPN(lexTable, idTable);

		////вывод LT
		lexTable.PrintTable();
		std::cout << "\n\n";


		//// сематниеский анализ
		SMTCS::Semantics smtcs(lexTable, idTable);
		smtcs.CheckOperatoinsSemantics();
		smtcs.CheckFunctionSemantics();

		//вывод IT
		idTable.PrintTable();

		// генерация кода
		AsmGen::Gen generrator(lexTable, idTable, TAVasmOutFile);
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message;	printf("\t(%d, %d)\n\n", e.intext.line, e.intext.col);

		Log::WriteError(log, e);
		Log::Close(log);
	}
	return 0;
}