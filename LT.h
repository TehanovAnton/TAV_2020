#pragma once
#define LEXEMA_FIXSIZE 1			// фиксированный размер лексемы
#define LT_MAXSIZE 4096				// максимальное количество строк в таблице лексем
#define LT_TILNULLIDX 0xffffffff	// нет элемента таблицы идентификаторов 
#define LEX_INTT 't'				// лексема для IntT
#define LEX_STRT 't'				// лексема для StrT
#define LEX_BOOLT 't'				// лексема для BoolT
#define LEX_ID	'i'					// лексема для идентификатороф
#define LEX_LITERAL 'l'				// лексема для литерала
#define LEX_DEFF 'f'				// лексема для function				
#define LEX_DEF 'd'					// лексема для declare					
#define LEX_RET 'r'					// лексема для return					
#define LEX_IF 'a'					// лексема для IF
#define LEX_RIF 'b'					// лексема для RIF	
#define LEX_PUT 'p'					// лексема для print						
#define LEX_SEMICOLON ';'			// лексема для ;						
#define LEX_COMMA ','				// лексема для ,							
#define LEX_NEWLINE	';'				// лексекма для \n
#define LEX_END 'e'					// лексемам для end 
#define LEX_LEFTHESIS '('			// лексема для (						
#define LEX_RIGHTHESIS ')'			// лексема для )						
#define LEX_PLUS 'v'				// лексема для +							
#define LEX_MINUS 'v'				// лексема для -							
#define LEX_STAR 'v'				// лексема для *							
#define LEX_DIRSLASH 'v'			// лексема для /
#define LEX_MORE 'w'				// лексема для >									 w => операция возвращает BoolT 
#define LEX_LESS 'w'				// лексема для <

#define LEX_BOOLEAQUl 'w'			// лексема для ==
#define LEX_BOOLEAQUl_OPER_V 'h'	// ==, oper_v => 'h'

#define LEX_MOREEQ 'w'				// лексема для >=
#define LEX_MOREEQ_OPER_V 'c'		// >=, oper_v => 'c'

#define LEX_LESSEQ 'w'				// лексема для <=
#define LEX_LESSEQ_OPER_V 'g'		// лексема для <=, oper_v => 'g'

#define LEX_EQUAl '='				// лексема для =
#define LEX_STARTPOINT 'm'			// лекскема для main
#define LEX_LSQBRACKET '['
#define LEX_RSQBRACKET ']'

#include "IT.h"
#include "SpecialSym.h"
#include "Error.h"
#include<iostream>
#include<string>
#include<queue>

bool isSymInStr(char sym, const char str[]);

namespace LT							// таблица лексем		
{
	struct Entry						// строка таблицы лексем
	{
		char lexema[LEXEMA_FIXSIZE + 1];	// лексема
		char oper_v;
		int sn;							// номер строки в исходном тексте
		int idxTI;						// индекс в таблице идентификаторов или LТ_ТI_NULLIDX
		int cn;						    // номер колонки в исходном коде

		Entry();

		Entry(char origlex, char shortLexm, int line, int idxTI);
	};

	struct Position						// структура хранит положение лекскемыв исходном тексте программы
	{
		unsigned int colone;
		unsigned int line;
		Position();
		Position(int cl, int ln);
	};

	struct LexTable						// экземпляр таблицы лексем
	{
		int maxsize;				    // ёмкость таблицы лексем < LT_MAXSIZE
		int size;						// текущий размер таблицы лексем < maxsize
		Entry* table;					// массив строк таблицы лексем
		int posSize;
		Position* positions;				// массив положений лексем

		int posLEX_EQUALSSize;
		int* posLEX_EQUALS;

		std::queue<int>posLEX_IF_RIF;		// очередь положений IF и RIF

		void GetLexemsPosition(std::string  originalText);		// функция заполняет принемает текст исходной и заполняет массив положежний
		void PrintTable();										// вывод таблицы
	};

	LexTable Create(								// создать таблицу лексем
		int size						// ёмкость таблицы лексем < LT_MAXSIZE
	);

	void Add(										// добавить строку в таблицу лексем
		LexTable& lextable,					// экземпляр таблицы лексем
		Entry& entry							// строка таблицы лексем
	);

	Entry GetEntry(
		LexTable& lextable,				// экземпляр таблицы лексем
		int n							// номер получаемой строки
	);

	int IstdByID(LexTable lexTable, char value[]);

	void Delete(									// удалить таблицу лексем (освободить память)
		LexTable& lextable
	);

	bool ITPointerBefore(LT::LexTable lexTable, int idx, int ITpointer);

	bool IsOperation(char shrtlex);
}