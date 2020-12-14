#pragma once
#include<iostream>
#include<cwchar>

#include"Error.h"
#include"Parm.h"
#include"In.h"
#include"Log.h"
#include"TCHAR.h"
#include<fstream>
#include"FST.h"
#include"IT.h"
#include"LT.h"
#include"PolishNotation.h"
#include "SpecialSym.h"

#define TOKENINIT 1

void setDefValue(IT::Entry& entryI)
{
	if (entryI.iddatatype == IT::INT)
		entryI.values.vint = 0;
	else
	{
		entryI.values.vste->len = 0;
		entryI.values.vste->str[0] = '\0';
	}
}

void setEntryIId(IT::Entry& entryI, char lexem[])
{
	size_t i = 0;
	for (; i < strlen(lexem) && i < ID_MAXSIZE; i++)
		entryI.id[i] = lexem[i];

	entryI.id[i] = '\0';
}

void parsStringLiteraltoValue(char lexem[], char retValue[])
{
	size_t e = 1;
	for (; e < strlen(lexem) - 1; e++)
	{
		retValue[e - 1] = lexem[e];
	}
	retValue[e - 1] = '\0';
}

char* IdbyLexem(char lexem[])
{
	char* ID = new char[ID_MAXSIZE + 1];
	size_t i = 0;
	for (; i < strlen(lexem) && i < ID_MAXSIZE; i++)
	{
		ID[i] = lexem[i];
	}
	ID[i] = '\0';

	return ID;
}

bool strcamper(char idtableEl[], char findingEl[])
{
	bool res = true;
	for (size_t i = 0; i < ID_MAXSIZE; i++)
	{
		res &= idtableEl[i] == findingEl[i];
	}

	return res;
}

bool strcamper(char idtableEl[], const char* findingEl)
{
	bool res = true;
	for (size_t i = 0; i < strlen(idtableEl) && i < ID_MAXSIZE; i++)
	{
		res &= idtableEl[i] == findingEl[i];
	}

	return res;
}

void set_aE(IT::IdTable& idTable, int lstChngId, int aB, int aE);
void set_aB(LT::LexTable lexTable, IT::IdTable& idTable, int& lstChngId, int aB, int& aE);
void exposingNamespaces(LT::LexTable lexTable, IT::IdTable& idTable);

void FillLT(LT::LexTable& lexTable, LT::Entry& entryL, char origLex[],  char shrtLex[], int line)
{
	// ���������� LT; ������ �� IT � ��� �������� ����� ��������������� ��� ���������� IT

	if (strlen(origLex) == 1)
		entryL = LT::Entry(origLex[0], shrtLex[0], line, LT_TILNULLIDX);
	else if(strcamper(origLex, moreEqual))
		entryL = LT::Entry('c', shrtLex[0], line, LT_TILNULLIDX);
	else if (strcamper(origLex, lessEqual))
		entryL = LT::Entry('g', shrtLex[0], line, LT_TILNULLIDX);
	else if (strcamper(origLex, boolEqual))
		entryL = LT::Entry('h', shrtLex[0], line, LT_TILNULLIDX);
	else
		entryL = LT::Entry('\0', shrtLex[0], line, LT_TILNULLIDX);

	LT::Add(lexTable, entryL);
}

bool FillIT(LT::LexTable& lexTable, IT::IdTable& idTable, IT::Entry& entryI, int lexCounter, char lexem[], int currentToken, int lastToken, int preLastToken, int& litCounter)
{
	entryI = IT::Entry();

	// ����������
	// �����������
	if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_DEF)
	{
		IT::IDDATATYPE dataType = lastToken == 1 ? IT::STR :
			lastToken == 2 ? IT::INT : IT::BOOL;

		entryI = IT::Entry(lexem, lexCounter, dataType, IT::V);

		// �������� ��������������� ������� � LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;
	}
	// �������
	// �����������
	else if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_DEFF)
	{
		entryI = IT::Entry(lexem, lexCounter, preLastToken == 1 ? IT::STR : IT::INT, IT::F);

		// �������� ��������������� ������� � LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;
	}
	// ��������
	// �����������
	else if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_LEFTHESIS || lexTable.table[lexTable.size - 3].lexema[0] == LEX_COMMA)
	{
		entryI = IT::Entry(lexem, lexCounter, lastToken == 1 ? IT::STR : IT::INT, IT::P);

		// �������� ��������������� ������� � LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;
	}
	// �������
	else if (currentToken >= 3 && currentToken <= 5)
	{
		if (currentToken >= 4 && currentToken <= 5) // IntT literal
		{
			// ������ ����� �� int literal
			if (IT::IstdIntTLitByValue(idTable, lexem) != (int)TI_NULLIDX)
			{
				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdIntTLitByValue(idTable, lexem);
				return true;
			}
			else // �� ������ IntT literal
			{
				if (currentToken == 5)
					entryI = IT::Entry(lexem, lexCounter, litCounter, IT::INT, IT::L, DECLITERAL);
				else if (currentToken == 6)
					entryI = IT::Entry(lexem, lexCounter, litCounter, IT::INT, IT::L, OCTLITERAL);
				else if (currentToken == 7)
					entryI = IT::Entry(lexem, lexCounter, litCounter, IT::INT, IT::L, HEXLITERAL);
				else
					entryI = IT::Entry(lexem, lexCounter, litCounter, IT::INT, IT::L, BINLITERAL);

				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
		else if (currentToken == 10)							 // BoolT - false
		{
			// ������ BoolT - false
			if (IT::IstdBoolTLitByValue(idTable, TI_FALSE_VAl) != (int)TI_NULLIDX)
			{
				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdBoolTLitByValue(idTable, TI_FALSE_VAl);
				return true;
			}
			else
			{
				entryI = IT::Entry(TI_FALSE_VAl, lexCounter, litCounter, IT::BOOL, IT::L);

				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
		else if (currentToken == 9)						 // BoolT - true
		{				  
			// ������ BoolT - true
			if (IT::IstdBoolTLitByValue(idTable, TI_TRUE_VAL) != (int)TI_NULLIDX)
			{
				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdBoolTLitByValue(idTable, TI_TRUE_VAL);
				return true;
			}
			else
			{
				entryI = IT::Entry(TI_TRUE_VAL, lexCounter, litCounter, IT::BOOL, IT::L);

				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
		else // StrT �������
		{
			char value[1000];
			parsStringLiteraltoValue(lexem, value);
			// ������ ����� �� string literal
			if (IT::IstdStrTLitByValue(idTable, value) != (int)TI_NULLIDX)
			{
				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdStrTLitByValue(idTable, value);
				return true;
			}
			// �� ������ string literal
			else
			{
				entryI = IT::Entry(value, lexCounter, litCounter, IT::STR, IT::L);

				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
	}
	else // ������������� ������ � �������
	{
		setEntryIId(entryI, lexem);
		if (IT::IstdByID(idTable, entryI.id) != (int)TI_NULLIDX)
		{
			lexTable.table[lexTable.size - 1].idxTI = IT::IstdFromCurrent(idTable, IdbyLexem(lexem));
			return true;
		}
		else
		{
			throw ERROR_THROW_IN(116, lexTable.positions[lexCounter - 1].line, lexTable.positions[lexCounter - 1].colone);
		}																																	  
	}

	IT::Add(idTable, entryI, lexTable.positions[lexCounter - 1].line, lexTable.positions[lexCounter - 1].colone);
	return true;
}

bool parsingLexem(char lexem[], LT::LexTable& lexTable, IT::IdTable& idTable, LT::Entry& entryL, IT::Entry& entryI, int line, int& litCounter, int& lastToken, int& preLastToken, int lexCounter)
{

#if TOKENINIT

	FST::FST ConcatStd(
		lexem,                                                                                                        // ������� ������������� 
		ConcatStdLibTOk);

	FST::FST CompareStd(
		lexem,                                                                                                        // ������� ������������� 
		CompareStdLibTOk);

	FST::FST ELSL(
		lexem,                                                                                                        // ������� ������������� 
		ELSKWTOk);

	FST::FST leftBracetL(
		lexem,                                                                                                        // ������� ������������� 
		leftBracetLTok);

	FST::FST rightBracetL(
		lexem,                                                                                                        // ������� ������������� 
		rightBracetLTok);

	FST::FST endL(
		lexem,                                                                                                        // ������� ������������� 
		endKWTok);

	FST::FST commaL(
		lexem,                                                                                                        // ������� ������������� 
		commaLTok);

	FST::FST plusL(
		lexem,                                                                                                        // ������� ������������� 
		plusLTok);

	FST::FST minusL(
		lexem,                                                                                                        // ������� ������������� 
		minusLTok);

	FST::FST multiplicationL(
		lexem,                                                                                                        // ������� ������������� 
		multiplicationLTok);

	FST::FST devisionL(
		lexem,                                                                                                        // ������� ������������� 
		devisionLTok);

	FST::FST equalL(
		lexem,                                                                                                        // ������� ������������� 
		equalLTok);

	FST::FST boolequalL(
		lexem,                                                                                                        // ������� ������������� 
		boolequalLTok);

	FST::FST lessL(
		lexem,                                                                                                        // ������� ������������� 
		lessLTok);

	FST::FST moreL(
		lexem,                                                                                                        // ������� ������������� 
		moreLTok);

	FST::FST moduloL(
		lexem,                                                                                                        // ������� ������������� 
		moduloLTok);

	FST::FST IntTKeyWord(
		lexem,
		IntTKWTOk);

	FST::FST StrTKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		StrTKWTok);

	FST::FST defFKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		defFKWTok);

	FST::FST defKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		defKWTok);

	FST::FST retKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		retKWTok);

	FST::FST IFKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		IFKWTok);

	FST::FST putKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		putKWTok);

	FST::FST StartPointKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		StartPointKWTok);

	FST::FST identificator(
		lexem,                                                                                                        // ������� ������������� 
		identificatorTok);

	FST::FST IntTDecLiteral(
		lexem,
		IntTDecLiteralTok);

	FST::FST IntTHexLiteral(
		lexem,
		IntTHexLiteralTok);

	FST::FST StrTLiteral(
		lexem,
		StrTLiteralTok);

	FST::FST newlineKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		newlinelLTok);

#endif // TOKENINIT																											 

	FST::FST tokenLexems[NUMLEXEM] =
		{ StartPointKeyWord,	StrTKeyWord,		IntTKeyWord,		StrTLiteral,	IntTDecLiteral, 
		IntTHexLiteral,			defFKeyWord,		moreL,				ELSL,			defKeyWord,
		retKeyWord,				putKeyWord,			endL,				leftBracetL,	rightBracetL,
		IFKeyWord,				commaL, 			plusL,				minusL,			multiplicationL,
		lessL,					boolequalL,			devisionL,			equalL,			newlineKeyWord,
		ConcatStd,				CompareStd,			moduloL,			identificator};

	char shortLexm[NUMLEXEM] = 
		{ LEX_ID,		LEX_STRT,			LEX_INTT,		LEX_LITERAL,	LEX_LITERAL,
		LEX_LITERAL,	LEX_DEFF,			LEX_MORE,		LEX_ElS,		LEX_DEF,
		LEX_RET,		LEX_PUT,			LEX_END,		LEX_LEFTHESIS,	LEX_RIGHTHESIS,
		LEX_IF,			LEX_COMMA,			LEX_PLUS,		LEX_MINUS,		LEX_STAR,
		LEX_LESS,		LEX_BOOLEAQUl,		LEX_DIRSLASH,	LEX_EQUAl,		LEX_NEWLINE,
		LEX_ID,			LEX_ID,				LEX_MODULOL,	LEX_ID};

	for (int i = 0; i < NUMLEXEM; i++)
	{
		if (FST::execute(tokenLexems[i]))
		{
			char StartPointKW[] = "StartPoint";
			char shrtLex[2] = { shortLexm[i] , '\0' };
			if (strcamper(lexem, StartPointKW))
			{
				shrtLex[0] = LEX_STARTPOINT;
				if (LT::IstdByID(lexTable, shrtLex) != (int)TI_NULLIDX)
				{
					throw ERROR_THROW_IN(117, line, lexTable.positions[lexCounter - 1].colone)
				}
			}
			// ���������� ������� IF RIF ��� PL
			if (shrtLex[0] == LEX_LEFTHESIS &&
				lexTable.table[lexTable.size - 1].lexema[0] == LEX_IF || lexTable.table[lexTable.size - 1].lexema[0] == LEX_RIF)
				lexTable.posLEX_EQUALS[lexTable.posLEX_EQUALSSize++] = lexTable.size;

			if (i == 25 || i == 26  && IT::IstdByID(idTable, entryI.id) == (int)TI_NULLIDX)
			{
				LT::Entry stdLib = entryL = LT::Entry('\0', shrtLex[0], line, LT_TILNULLIDX);
				stdLib.sn = line, stdLib.cn = 0, stdLib.idxTI = idTable.size, stdLib.lexema[0] = LEX_ID, stdLib.oper_v = '\0';
				LT::Add(lexTable, stdLib);

				if (i == 25)
				{									
					IT::Entry stdIDLib = IT::Entry(lexem, lexCounter, IT::IDDATATYPE::STR, IT::IDTYPE::F);
					IT::Add(idTable, stdIDLib, lexTable.positions[lexCounter - 1].line, lexTable.positions[lexCounter - 1].colone);
				}
				else
				{
					IT::Entry  stdIDLib = IT::Entry(lexem, lexCounter, IT::IDDATATYPE::INT, IT::IDTYPE::F);
					IT::Add(idTable, stdIDLib, lexTable.positions[lexCounter - 1].line, lexTable.positions[lexCounter - 1].colone);
				}

				preLastToken = lastToken;
				lastToken = i;

				return true;
			}
			else if(i == 25 || i == 26 && IT::IstdByID(idTable, entryI.id) != (int)TI_NULLIDX)
			{
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdFromCurrent(idTable, IdbyLexem(lexem));
				return true;
			}

			FillLT(lexTable, entryL,  lexem, shrtLex, line);

			if (shrtLex[0] == LEX_ID || shrtLex[0] == LEX_LITERAL)
			{
				FillIT(lexTable, idTable, entryI, lexCounter, lexem, i, lastToken, preLastToken, litCounter);
			}

			preLastToken = lastToken;
			lastToken = i;

			return true;
		}
		else
			continue;
	}

	throw ERROR_THROW_IN(115, line, lexTable.positions[lexCounter - 1].colone)

	return false;
}

std::string processText(std::string str)
{
	std::string resStr = "";
	for (size_t i = 0; i < str.length(); i++)
	{
		// ������ ����� ���������
		if (str[i] == quotation)
		{
			resStr += str[i];
			i++;

			for (; str[i] != quotation; i++)
				resStr += str[i];

			resStr += str[i];
			continue;
		}

		if (!isSymInStr(str[i], spaceAndNewlineSym))
		{
			// ������ ������ �������� ����� �����������
			if (isSymInStr(str[i], separators) && isSymInStr(str[i + 1], spaceSym))
			{
				resStr += str[i];
				i++;
				continue;
			}

			resStr += str[i];
		}
		else
		{
			// ������ ������ �������� ����� ������������
			if (isSymInStr(str[i], spaceSym) && isSymInStr(str[i + 1], separators))
			{
				continue;
			}

			resStr += str[i];
			// ������ ������ �������� ����� ������������
			while (isSymInStr(str[i + 1], spaceAndNewlineSym)) i++;
		}
	}

	return resStr;
}

void LTITBuilding(LT::LexTable& lexTable, IT::IdTable& idTable, std::string orifginalStr)
{
	char longLexem[TI_STR_MAXSIZE];	
	int litCounter = 0, lastToken = 0, preLastToken = 0, lexCounter = 0;
	LT::Entry entryL;
	IT::Entry entryI;
	for (size_t i = 0, e = 0, lexCounter = 0; i < orifginalStr.length() + 1; i++, e++)
	{
		if (isSymInStr(orifginalStr[i], sepSpaNewlSym) || i == orifginalStr.length())
		{
			if (e != 0)
			{
				parsingLexem(longLexem, lexTable, idTable, entryL, entryI, lexTable.positions[lexCounter++].line, litCounter, lastToken, preLastToken, lexCounter);
			}

			// ��������� �� ���������� ������������
			if (i != orifginalStr.length() && !isSymInStr(orifginalStr[i], spaceAndNewlineSym))
			{
				char lexemShrt[2] = { orifginalStr[i], '\0' };
				char lexemLng[3] = { orifginalStr[i], orifginalStr[i + 1],  '\0' };

				parsingLexem(
					isSymInStr(orifginalStr[i], moreLessEqual) && orifginalStr[i + 1] == LEX_EQUAl ? i++, lexemLng : lexemShrt,
					lexTable, idTable, entryL, entryI, lexTable.positions[lexCounter++].line, litCounter, lastToken, preLastToken, lexCounter);

				if (orifginalStr[i] == LEX_EQUAl && orifginalStr[i + 1] != LEX_EQUAl && orifginalStr[i - 1] != LEX_EQUAl)
					lexTable.posLEX_EQUALS[lexTable.posLEX_EQUALSSize++] = lexTable.size - 1;								
			}

			//����� ����� ������
			if (orifginalStr[i] == newlSym)
			{
				e = -1;
				char lexem[2] = { ';', '\0'};
				parsingLexem(lexem, lexTable, idTable, entryL, entryI, lexTable.positions[lexCounter++].line, litCounter, lastToken, preLastToken, lexCounter);
			}
			else
			{
				e = -1;
				memset(longLexem, 0, sizeof(char) * TI_STR_MAXSIZE);
			}
		}
		// ���������� �������
		else
		{
			// ������ ����� �������
			if (orifginalStr[i] == quotation)
			{
				longLexem[e] = orifginalStr[i]; i++; e++;
				longLexem[e + 1] = '\0';
				while (orifginalStr[i] != quotation)
				{
					longLexem[e] = orifginalStr[i];
					longLexem[e + 1] = '\0';
					i++; e++;
				}
				longLexem[e] = orifginalStr[i];
				longLexem[e + 1] = '\0';
				continue;
			}
			else
			{
				longLexem[e] = orifginalStr[i];
				longLexem[e + 1] = '\0';
			}
		}
	}
	for (int i = 0; i < lexTable.size; i++)
	{
		lexTable.table[i].cn = lexTable.positions[i].colone;
	}
}

// area begin
void set_aB(LT::LexTable lexTable, IT::IdTable& idTable, int& lstChngId, int aB, int& aE)
{
	for (int i = aB + 1;
		lexTable.table[i].lexema[0] != LEX_END && lexTable.table[i].lexema[0] != LEX_ElS &&			// ������� end, ElS 
		i < lexTable.size;										// ����� LT
		i++, aE = i)
	{
		LT::Entry elLT = lexTable.table[i];
		// ��������� ��������� ������� ��������� 
		if (elLT.lexema[0] == LEX_ID && lexTable.table[i - 2].lexema[0] == LEX_DEFF || elLT.lexema[0] == LEX_STARTPOINT ||
			elLT.lexema[0] == LEX_IF || elLT.lexema[0] == LEX_RIF || lexTable.table[i - 1].lexema[0] == LEX_ElS)
		{
			set_aB(lexTable, idTable, lstChngId, i, aE);			

			set_aE(idTable, lstChngId, i, aE);

			i = aE;
		}
		else
		{
			if (elLT.lexema[0] == LEX_ID)
			{ 
				if (idTable.table[elLT.idxTI].vsbAr.aB == NOVSBAR && idTable.table[elLT.idxTI].idtype == IT::IDTYPE::V || idTable.table[elLT.idxTI].idtype == IT::IDTYPE::P)
				{
					idTable.table[elLT.idxTI].vsbAr.aB = aB;
					lstChngId = elLT.idxTI;
				}
			}
			
			else
			{ continue; }
		}
	}
}
// area end
void set_aE(IT::IdTable& idTable, int lstChngId, int aB, int aE)
{
	for (int i = lstChngId; i >= 0; i--)
	{
		if (idTable.table[i].vsbAr.aB == aB)
		{
			idTable.table[i].vsbAr.aE = aE; 
		}
	}
}


bool operator <= (IT::visibleArea a, IT::visibleArea b)
{
	return (a.aB > b.aB && b.aE < a.aE) || (a.aB == b.aB && a.aE == b.aE);
}
bool operator != (IT::visibleArea a, IT::visibleArea b)
{
	return (a.aB < b.aB&& a.aE < b.aE) || (a.aB > b.aB && a.aE > b.aE);
}
bool operator == (IT::visibleArea a, IT::visibleArea b)
{
	return a.aB == b.aB&& a.aE == b.aE;
}

void exposingNamespaces(LT::LexTable lexTable, IT::IdTable& idTable)
{
	int lasatChangedIdentificator = 0;
	for (int i = 0; i < lexTable.size; i++)
	{
		LT::Entry elLT = lexTable.table[i];
		if (elLT.lexema[0] == LEX_ID && lexTable.table[i - 2].lexema[0] == LEX_DEFF ||
			elLT.lexema[0] == LEX_STARTPOINT ||
			elLT.lexema[0] == LEX_IF || elLT.lexema[0] == LEX_RIF)
		{
			int aB = i;
			set_aB(lexTable, idTable, lasatChangedIdentificator, aB, i);

			set_aE(idTable, lasatChangedIdentificator, aB, i);
		}
	}

	// �������� �� ������ ���������������
	for (int i = 0; i < idTable.size; i++)
	{
		IT::Entry elIT = idTable.table[i];
		for (int e = i - 1; e >= 0; e--)
		{
			if (!(elIT.vsbAr != idTable.table[e].vsbAr) && elIT.idtype == idTable.table[e].idtype)
			{
				if (strcamper(elIT.id, idTable.table[e].id) && elIT.vsbAr <= idTable.table[e].vsbAr)
				{
					printf("\n\n???: %s :???", elIT.id);
					throw ERROR_THROW_IN(125, lexTable.positions[elIT.idxfirstLE - 1].line, lexTable.positions[elIT.idxfirstLE - 1].colone)			
				}
			}
		}

	}

}
void ChangeLTWithPN(LT::LexTable& lexTable, IT::IdTable& idTable)
{
	// ��������� ��������� � �������� �������
	int countEmptyEl = 0;
	for (int i = 0; i < lexTable.posLEX_EQUALSSize; i++)
	{
		lexTable.table[lexTable.posLEX_EQUALS[i] - 1].lexema[0] == LEX_IF || lexTable.table[lexTable.posLEX_EQUALS[i] - 1].lexema[0] == LEX_RIF ? 
			PN::PolishNotation(lexTable.posLEX_EQUALS[i], lexTable, idTable, countEmptyEl, true) :
			PN::PolishNotation(lexTable.posLEX_EQUALS[i], lexTable, idTable, countEmptyEl);
	}
	// ���������� ������ ���������
	LT::Entry* cleanTable = new  LT::Entry[lexTable.size - countEmptyEl];
	for (int i = 0, e = 0, poseqindx = 0; i < lexTable.size; i++)
	{
		if (lexTable.table[i].lexema[0] != EMPTYSTR)
		{
			cleanTable[e++] = lexTable.table[i];

			// �������� ������� ����� �����
			if (cleanTable[e - 1].lexema[0] == LEX_EQUAl)
				lexTable.posLEX_EQUALS[poseqindx++] = e - 1;
		}
		else
		{
			continue;
		}
	}
	// ������� ������
	delete[] lexTable.table;
	lexTable.table = cleanTable;	
	lexTable.size = lexTable.size - countEmptyEl;
}
