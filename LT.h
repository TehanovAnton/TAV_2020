#pragma once
#define LEXEMA_FIXSIZE 1			// ������������� ������ �������
#define LT_MAXSIZE 4096				// ������������ ���������� ����� � ������� ������
#define LT_TILNULLIDX 0xffffffff	// ��� �������� ������� ��������������� 
#define LEX_INTT 't'				// ������� ��� IntT
#define LEX_STRT 't'				// ������� ��� StrT
#define LEX_BOOLT 't'				// ������� ��� BoolT
#define LEX_ID	'i'					// ������� ��� ���������������
#define LEX_LITERAL 'l'				// ������� ��� ��������
#define LEX_DEFF 'f'				// ������� ��� function				
#define LEX_DEF 'd'					// ������� ��� declare					
#define LEX_RET 'r'					// ������� ��� return					
#define LEX_IF 'a'					// ������� ��� IF
#define LEX_RIF 'b'					// ������� ��� RIF	
#define LEX_PUT 'p'					// ������� ��� print						
#define LEX_SEMICOLON ';'			// ������� ��� ;						
#define LEX_COMMA ','				// ������� ��� ,							
#define LEX_NEWLINE	';'				// �������� ��� \n
#define LEX_END 'e'					// �������� ��� end 
#define LEX_LEFTHESIS '('			// ������� ��� (						
#define LEX_RIGHTHESIS ')'			// ������� ��� )						
#define LEX_PLUS 'v'				// ������� ��� +							
#define LEX_MINUS 'v'				// ������� ��� -							
#define LEX_STAR 'v'				// ������� ��� *							
#define LEX_DIRSLASH 'v'			// ������� ��� /
#define LEX_MORE 'w'				// ������� ��� >									 w => �������� ���������� BoolT 
#define LEX_LESS 'w'				// ������� ��� <

#define LEX_BOOLEAQUl 'w'			// ������� ��� ==
#define LEX_BOOLEAQUl_OPER_V 'h'	// ==, oper_v => 'h'

#define LEX_MOREEQ 'w'				// ������� ��� >=
#define LEX_MOREEQ_OPER_V 'c'		// >=, oper_v => 'c'

#define LEX_LESSEQ 'w'				// ������� ��� <=
#define LEX_LESSEQ_OPER_V 'g'		// ������� ��� <=, oper_v => 'g'

#define LEX_EQUAl '='				// ������� ��� =
#define LEX_STARTPOINT 'm'			// �������� ��� main
#define LEX_LSQBRACKET '['
#define LEX_RSQBRACKET ']'

#include "IT.h"
#include "SpecialSym.h"
#include "Error.h"
#include<iostream>
#include<string>
#include<queue>

bool isSymInStr(char sym, const char str[]);

namespace LT							// ������� ������		
{
	struct Entry						// ������ ������� ������
	{
		char lexema[LEXEMA_FIXSIZE + 1];	// �������
		char oper_v;
		int sn;							// ����� ������ � �������� ������
		int idxTI;						// ������ � ������� ��������������� ��� L�_�I_NULLIDX
		int cn;						    // ����� ������� � �������� ����

		Entry();

		Entry(char origlex, char shortLexm, int line, int idxTI);
	};

	struct Position						// ��������� ������ ��������� ��������� �������� ������ ���������
	{
		unsigned int colone;
		unsigned int line;
		Position();
		Position(int cl, int ln);
	};

	struct LexTable						// ��������� ������� ������
	{
		int maxsize;				    // ������� ������� ������ < LT_MAXSIZE
		int size;						// ������� ������ ������� ������ < maxsize
		Entry* table;					// ������ ����� ������� ������
		int posSize;
		Position* positions;				// ������ ��������� ������

		int posLEX_EQUALSSize;
		int* posLEX_EQUALS;

		std::queue<int>posLEX_IF_RIF;		// ������� ��������� IF � RIF

		void GetLexemsPosition(std::string  originalText);		// ������� ��������� ��������� ����� �������� � ��������� ������ ����������
		void PrintTable();										// ����� �������
	};

	LexTable Create(								// ������� ������� ������
		int size						// ������� ������� ������ < LT_MAXSIZE
	);

	void Add(										// �������� ������ � ������� ������
		LexTable& lextable,					// ��������� ������� ������
		Entry& entry							// ������ ������� ������
	);

	Entry GetEntry(
		LexTable& lextable,				// ��������� ������� ������
		int n							// ����� ���������� ������
	);

	int IstdByID(LexTable lexTable, char value[]);

	void Delete(									// ������� ������� ������ (���������� ������)
		LexTable& lextable
	);

	bool ITPointerBefore(LT::LexTable lexTable, int idx, int ITpointer);

	bool IsOperation(char shrtlex);
}