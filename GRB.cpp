#include"GRB.h"

namespace GRB				// S N E M F W
{
	//S -> fm; NrE; e | fti(F); NrE; e; S | fm; NrE; e; S | * tfi(F) { NrE; };
	Greibach greibach(
		NS('S'),										 // стартовый символ
		TS('$'),										 // дно стека
		6,												 // количество правил
		Rule(NS('S'), GRB__ERROR_SERIES + 0,			 // неверная структруа программы
			3,
			Rule::Chain(8, TS('f'), TS('m'), TS(';'), NS('N'), TS('r'), NS('E'), TS(';'), TS('e')),
			Rule::Chain(14, TS('f'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N'), TS('r'), NS('E'), TS(';'), TS('e'), TS(';'), NS('S')),
			Rule::Chain(9, TS('f'), TS('m'), TS(';'), NS('N'), TS('r'), NS('E'), TS(';'), TS('e'), TS(';'), NS('S'))
			//Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'))
		),																					   
		/* N -> dti(F); | dti(F); N | dti = E; | dti = E; N | dti; N | dti; | rE; N | rE; | i = E; N | i = E; | dtfi(F); N | dtfi(F);
		   | pE; | pE; N |
		   a(E); N e; | a(E); N e; N |					 // условный опрератор
		   b(E); N e; | b(E); N e; N					 // опрератор цикла
		*/
		Rule(NS('N'), GRB__ERROR_SERIES + 1,			 // ошибочный опрератор
			16,
			//Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			//Rule::Chain(8, TS('d'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(8, TS('f'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(7, TS('f'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(9, TS('a'), TS('('), NS('E'), TS(')'), TS(';'), NS('N'), TS('e'), TS(';'), NS('N')),
			Rule::Chain(8, TS('a'), TS('('), NS('E'), TS(')'), TS(';'), NS('N'), TS('e'), TS(';')),
			Rule::Chain(9, TS('b'), TS('('), NS('E'), TS(')'), TS(';'), NS('N'), TS('e'), TS(';'), NS('N')),
			Rule::Chain(8, TS('b'), TS('('), NS('E'), TS(')'), TS(';'), NS('N'), TS('e'), TS(';'))
		),
		//E -> i | l | (E) | i(W) | iM | lM | (E)M | i(W)M
		Rule(NS('E'), GRB__ERROR_SERIES + 2,
			8,											 // ошибка в выражении
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		//M -> vE |  v(E) | vEM 
		Rule(NS('M'), GRB__ERROR_SERIES + 3,
			3,
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(4, TS('v'), TS('('), NS('E'), TS(')')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),
		//F -> ti | ti, F
		Rule(NS('F'), GRB__ERROR_SERIES + 4,
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		//W -> i | l | i, W | l, W
		Rule(NS('W'), GRB__ERROR_SERIES + 5,
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; i++)
		{
			nt[i] = (GRBALPHABET)p[i];
		}
	}

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++)
		{
			chains[i] = p[i];
		}
	}

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (short i = 0; i < size; i++)
		{
			rules[i] = p[i];
		}
	}

	Greibach getGreibach()
	{
		return greibach;
	};

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)   // переходим дальше по правила до первого совпадения нетерминалов
		{
			k++;
		}
		if (k < size)
		{
			prule = rules[rc = k];
		}
		return rc;
	};

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain)								  // получить правило в виде N -> цепочка
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pChain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)		 // смотрим по очереди среди цепочек до первого совпадения начальных символов
		{
			++j;
		}
		rc = j < size ? j : -1;							 // если вышли запределы размера то вернуть -1
		if (rc >= 0)
		{
			pChain = chains[rc];
		}
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)													  // полуить правую сторону правила
	{
		for (int i = 0; i < size; i++)
		{
			b[i] = Chain::alphabet_to_char(nt[i]);
		}
		b[size] = 0x00;
		return b;
	}
}
