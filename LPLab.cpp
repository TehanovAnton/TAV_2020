#include"LPLab.h"
#include "PolishNotation.h"
#include"MFST.h"

#define LPLab 1	
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{		
#if LPLab
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

		exposingNamespaces(lexTable, idTable);

		//вывод LT
		/*lexTable.PrintTable();
		std::cout << "\n\n\n";*/

		////LPLab16
		MFST_TRACE_START											   // отладка
		MFST::Mfst mfst(lexTable, GRB::getGreibach());			   // автомат
		mfst.start();												   // старт синтаксического анализа

		////LPLab15
		ChangeLTWithPN(lexTable, idTable);

		////вывод LT
		lexTable.PrintTable();

		//вывод IT
		idTable.PrintTable();

		Log::Close(log);

#endif // LPLab
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message;	printf("\t(%d, %d)\n\n", e.intext.line, e.intext.col);

		Log::WriteError(log, e);
		Log::Close(log);
	}
	return 0;
}