#include"GRB.h"

namespace GRB				// S N E M F W Y Z
{
	//S -> fm; rE; e | fm; NrE; e | fm; NrE; e; S | fti(F); NrE; e; S | fti(F); S
	Greibach greibach(
		NS('S'),										 // стартовый символ
		TS('$'),										 // дно стека
		9,												 // количество правил
		Rule(NS('S'), GRB__ERROR_SERIES + 0,			 // неверная структруа программы
			5,
			Rule::Chain(7, TS('f'), TS('m'), TS(';'), TS('r'), NS('E'), TS(';'), TS('e')),
			Rule::Chain(8, TS('f'), TS('m'), TS(';'), NS('N'), TS('r'), NS('E'), TS(';'), TS('e')),
			Rule::Chain(9, TS('f'), TS('m'), TS(';'), NS('N'), TS('r'), NS('E'), TS(';'), TS('e'), TS(';'), NS('S')),
			Rule::Chain(14, TS('f'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N'), TS('r'), NS('E'), TS(';'), TS('e'), TS(';'), NS('S')),
			Rule::Chain(8, TS('f'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('S'))
		),																					   
		/* N -> dti = Z; | dti = Z; N |
		   dti = E; | dti = E; N  |
		   dti = X; | dti = X; N  |
		   ri; N | ri; | rl; N | rl; | i = E; N | i = E; | i = Z; N | i = Z;
		   i = X; N | i = X;
		   pE; | pE; N |
		   a(Z); N e; | a(Z); N e; N |					 // условный опрератор
		   j; N											 // els переход
		   b(Z); N e; | b(Z); N e; N |					 // опрератор цикла
		   i(W); N |
		   i(W);
		*/
		Rule(NS('N'), GRB__ERROR_SERIES + 1,			 // ошибочный опрератор
			25,																			   
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('Z'), TS(';')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('Z'), TS(';'), NS('N')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('X'), TS(';')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('X'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), TS('i'), TS(';')),
			Rule::Chain(4, TS('r'), TS('l'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), TS('l'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('Z'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('Z'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('X'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('X'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(9, TS('a'), TS('('), NS('Z'), TS(')'), TS(';'), NS('N'), TS('e'), TS(';'), NS('N')),
			Rule::Chain(8, TS('a'), TS('('), NS('Z'), TS(')'), TS(';'), NS('N'), TS('e'), TS(';')),
			Rule::Chain(3, TS('j'), TS(';'), NS('N')),
			Rule::Chain(9, TS('b'), TS('('), NS('Z'), TS(')'), TS(';'), NS('N'), TS('e'), TS(';'), NS('N')),
			Rule::Chain(8, TS('b'), TS('('), NS('Z'), TS(')'), TS(';'), NS('N'), TS('e'), TS(';')),
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';'))
		),
		//E -> i | l | (E) | iM | lM | (E)M | i(W)M | i(W) | Z
		Rule(NS('E'), GRB__ERROR_SERIES + 2,
			8,											 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),					
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')'))
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
		),
		// Z -> i | l | iY
		Rule(NS('Z'), GRB__ERROR_SERIES + 6,
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('i'), NS('Y')),
			Rule::Chain(2, TS('l'), NS('Y'))			
		),		

		// Y -> wi | wl				w -> booloper
		Rule(NS('Y'), GRB__ERROR_SERIES + 7,
			2,
			Rule::Chain(2, TS('w'), TS('l')),
			Rule::Chain(2, TS('w'), TS('i'))
		),

		// X ->i(W)
		Rule(NS('X'), GRB__ERROR_SERIES + 8,
			1,
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')'))
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
