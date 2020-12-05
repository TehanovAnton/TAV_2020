#include "Error.h"
namespace Error
{
	// ñåğèè îøèáîê:	0 - 99	ñèñòåìíûå îøèáêè
	//				100 - 109	îøèáêè ïàğàìåòğîâ
	//				110 - 119	îøèáêè îòêğûòèÿ è ÷òåíèÿ ôàéëîâ 
	ERROR errors[ERROR_MAX_ENTRY] = // òàáëèöà îøèáîê
	{
		ERROR_ENTRY(0, "Íåäîïóñòèìûé êîä îøèáêè"),	//êîä îøèáêè âíå äèàïàçîíà 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "ñèñòåìíûé ñáîé"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100, "ïàğàìåòğ -in, äîëæåí áûòü çàäàí"),
		ERROR_ENTRY(101, ""),
		ERROR_ENTRY(102, "ïàğàìåòğ -in, äîëæåí áûòü çàäàí"),
		ERROR_ENTRY(103, "îøèáêà îòêğûòèÿ ôàéëà ñèñõîäíûì êîäîì (-in)"),
		ERROR_ENTRY(104, "îøèáêà ïğè ñîçäàíèè ôàéëà ïğîòîêîëà (-log)"),
		ERROR_ENTRY(105, "îøèáêà ïğè ñîçäàíèè ôàéëà ïğîòîêîëà (-log)"),
		ERROR_ENTRY(106, ""),
		ERROR_ENTRY(107, ""),
		ERROR_ENTRY(108, ""),
		ERROR_ENTRY(109, ""),

		ERROR_ENTRY(110, ""),  													// îøèáêè ëåêñè÷åñêîãî àíàëèçà
		ERROR_ENTRY(111, "íåäîïóñòèìûé ñèìâîë â èñõîäíîì ôàéëå (-in)"),
		ERROR_ENTRY(112, ""),
		ERROR_ENTRY(113, "ïåğåïîëíåíèÿ òàáëèöû ëåêñåì"),
		ERROR_ENTRY(114, "ïåğåïîëíåíèÿ òàáëèöû èäåíòèôèêàòîğîâ"),
		ERROR_ENTRY(115, "íåğàñïîçíîíàÿ ëåêñåìà"),
		ERROR_ENTRY(116, "íåîáúÿâëåííûé èäåíòèôèêàòîğ"),
		ERROR_ENTRY(117, "ïåğåîïğåäåëåíèå main"),

		ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
															 
		ERROR_ENTRY(120, "íåñîîòâåòñòâèå òèïîâ îïåğàíäîâ â âûğàæåíèè"),			 // îøèáêè òèïîâ
		ERROR_ENTRY(121, "íåñîîòâåòñòâèå òèïîâ îïåğàöèé â âûğàæåíèè"),
		ERROR_ENTRY(122, "íåñîîòâåòñòâèå òèïà ïğèñâàåìîãî çíà÷åíèÿ"),
		ERROR_ENTRY(123, "íåñîîòâåòñòâèå òèïà ïàğàìåòğà ïğè âûçîâå ôóíêöèè"),
		ERROR_ENTRY(124, "íåâåğíîå êîëè÷åñòâî ïàğàìåòğîâ ïğè âûçîâå ôóíêöèè"),
		ERROR_ENTRY(125, ""),
		ERROR_ENTRY(126, ""),
		ERROR_ENTRY(127, ""),
		ERROR_ENTRY(128, ""),
		ERROR_ENTRY(129, ""),

		ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),

		ERROR_ENTRY(600, "íåâåğíàÿ ñòğóêòóğà ïğîãğàììû"),					// îøèüêè ñèíòàêñèñà
		ERROR_ENTRY(601, "Îøèáî÷íûé îïåğàòîğ"),
		ERROR_ENTRY(602, "Îøèáêà â âûğàæåíèè"),
		ERROR_ENTRY(603, "îøèáêà â ïàğàììåòğàõ ôíêöèè"),
		ERROR_ENTRY(604, "îøèáêà â ïàğàììåòğàõ âûçûâàåìîé ôíêöèè"),

		ERROR_ENTRY_NODEF(605), ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(608), ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),

		ERROR_ENTRY(700, "îòñóòñòâóşÿùàÿ îïåğåöèÿ Semantics.cpp"),					// îøèáêè ğàáîòû êîìïèëÿòîğà
		ERROR_ENTRY(701, ""),
		ERROR_ENTRY(702, ""),
		ERROR_ENTRY(703, ""),
		ERROR_ENTRY(704, ""),

		ERROR_ENTRY_NODEF10(800), ERROR_ENTRY_NODEF10(900)
	};

	ERROR geterror(int id)
	{
		bool rangeId = (id > 0 && id < ERROR_MAX_ENTRY) ? true : false;
		if (rangeId)
		{
			for (size_t i = 0; i < ERROR_MAX_ENTRY; i++)
				if (id == errors[i].id || -id == errors[i].id)
				{
					return errors[i];
				}
		}
		else
		{
			return errors[0];
		}
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		bool rangeId = (id > 0 && id < ERROR_MAX_ENTRY) ? true : false;
		if (rangeId)
		{
			for (size_t i = 0; i < ERROR_MAX_ENTRY; i++)
				if (id == errors[i].id || -id == errors[i].id)
				{
					errors[i].intext.col = col;
					errors[i].intext.line = line;
					return errors[i];
				}
		}
		else
		{
			return errors[0];
		}
	}
}
