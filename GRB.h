#pragma once
#include "Error.h"
#include<iostream>
#define GRB__ERROR_SERIES 600
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

typedef short GRBALPHABET;												// символы алфавита граматики, терминалы > 0, нетерминалы < 0

namespace GRB
{
	struct Rule	 														// правило в граматике Грейбах
	{
		GRBALPHABET nn;													// нетерминал (левый символ правила) < 0
		int iderror;													// идентификатор диагностического сообщения
		short size;														// количество цепочек - правых частей правила 

		struct Chain													// цепочка (правая часть правила)
		{
			short size;													// длина цепочки
			GRBALPHABET* nt;											// цепочка терминалов (> 0) и нетерминалов (< 0)

			Chain() { size = 0; nt = 0; }

			Chain(														
				short psize, 											// количество символов в цепочке
				GRBALPHABET s, ...										// символы (терминалы и нетерминалы)
			); 

			char* getCChain(char* b);									// получить правую часть правила

			static GRBALPHABET T(char t) { return GRBALPHABET(t); };	// терминал 

			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }; 	// нетерминал

			static bool isT(GRBALPHABET s) { return s > 0; };  			// терминвал?

			static bool isN(GRBALPHABET s) { return !isT(s); } 			// нетерминал?

			static char alphabet_to_char(GRBALPHABET s)					// GRBALPHABET->char
			{ 
				return isT(s) ? char(s) : char(-s);
			};	

		}*chains; 														// массив цепочек (правых частей правил)

		Rule() { nn = 0x00; size = 0; }

		Rule(
			GRBALPHABET pnn, 											// нетерминал (< 0)
			int iderror, 												// идентификатор диагностического сообщения
			short psize,												// количесвто цепочек - правы частей прпавли
			Chain c, ...												// множесвто цепочек - правы частей прпавли
		); 

		char* getCRule(													// подучить правил в виде N->цепочка (для распечатки)
			char* b, 													// буфер 
			short nchain 												// номер цепочки (правой чассти) в правиле
		);

		short getNextChain( 											// получить следующую за j подходящую цепочку, вернуть её номер или миус -1
			GRBALPHABET t,												// первый символ цепочки
			Chain& pchain,												// ввозращаемамя цепочка 
			short j														// номер цепочки 
		);
	};

	struct Greibach 													// граматика Грейбах
	{
		short size; 													// количесво правил 
		GRBALPHABET startN;												// стартовый символ 
		GRBALPHABET stbottomT;											// днро стека 
		Rule* rules; 													// мнжество правил
		Greibach()
		{
			short size = 0;
			size = 0;
			startN = 0;
			stbottomT = 0;
			rules = 0;
		};

		Greibach(
			GRBALPHABET pstartN, 										// стартовый символ 
			GRBALPHABET pstbottomT,									    // дно стека 
			short psize, 												// количество правил
			Rule r, ...													// праила
		);	  	

		short getRule(													// полуучить правило, вощвращается номер правиола или -1
			GRBALPHABET pnn, 											// левый сисвол правила
			Rule& prule 												// возвращаемое павило граматики
		);
		Rule getRule(short n); 											// получть правило по номеру
	};

	Greibach getGreibach(); 											// получить граматику 
};