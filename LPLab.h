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
	// добавление LT; ссылка на IT и имя литерала будут скоректированны при заполнении IT

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

	// переменная
	// объявляется
	if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_DEF)
	{
		IT::IDDATATYPE dataType = lastToken == 1 ? IT::STR :
			lastToken == 2 ? IT::INT : IT::BOOL;

		entryI = IT::Entry(lexem, lexCounter, dataType, IT::V);

		// корекция соответствующей лексемы в LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;
	}
	// функция
	// объявляется
	else if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_DEFF)
	{
		entryI = IT::Entry(lexem, lexCounter, preLastToken == 1 ? IT::STR : IT::INT, IT::F);

		// корекция соответствующей лексемы в LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;
	}
	// параметр
	// объявляется
	else if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_LEFTHESIS || lexTable.table[lexTable.size - 3].lexema[0] == LEX_COMMA)
	{
		entryI = IT::Entry(lexem, lexCounter, lastToken == 1 ? IT::STR : IT::INT, IT::P);

		// корекция соответствующей лексемы в LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;
	}
	// литерал
	else if (currentToken == 4 || currentToken == 5 || currentToken == 11 || currentToken == 10)
	{
		if (currentToken == 5) // IntT literal
		{
			// найден такой же int literal
			if (IT::IstdIntTLitByValue(idTable, atoi(lexem)) != (int)TI_NULLIDX)
			{
				// корекция соответствующей лексемы в LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdIntTLitByValue(idTable, atoi(lexem));
				return true;
			}
			else // не найден IntT literal
			{
				entryI = IT::Entry(atoi(lexem), lexCounter, litCounter, IT::INT, IT::L);

				// корекция соответствующей лексемы в LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
		else if (currentToken == 11)							 // BoolT - false
		{
			// найден BoolT - false
			if (IT::IstdBoolTLitByValue(idTable, TI_FALSE_VAl) != (int)TI_NULLIDX)
			{
				// корекция соответствующей лексемы в LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdBoolTLitByValue(idTable, TI_FALSE_VAl);
				return true;
			}
			else
			{
				entryI = IT::Entry(TI_FALSE_VAl, lexCounter, litCounter, IT::BOOL, IT::L);

				// корекция соответствующей лексемы в LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
		else if (currentToken == 10)						 // BoolT - true
		{				  
			// найден BoolT - true
			if (IT::IstdBoolTLitByValue(idTable, TI_TRUE_VAL) != (int)TI_NULLIDX)
			{
				// корекция соответствующей лексемы в LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdBoolTLitByValue(idTable, TI_TRUE_VAL);
				return true;
			}
			else
			{
				entryI = IT::Entry(TI_TRUE_VAL, lexCounter, litCounter, IT::BOOL, IT::L);

				// корекция соответствующей лексемы в LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
		else // StrT литерал
		{
			char value[1000];
			parsStringLiteraltoValue(lexem, value);
			// найден такой же string literal
			if (IT::IstdStrTLitByValue(idTable, value) != (int)TI_NULLIDX)
			{
				// корекция соответствующей лексемы в LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdStrTLitByValue(idTable, value);
				return true;
			}
			// не найден string literal
			else
			{
				entryI = IT::Entry(value, lexCounter, litCounter, IT::STR, IT::L);

				// корекция соответствующей лексемы в LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
	}
	else // идентификатор ищется в таблице
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

	FST::FST leftBracetL(
		lexem,                                                                                                        // ципочка распознавания 
		leftBracetLTok);

	FST::FST rightBracetL(
		lexem,                                                                                                        // ципочка распознавания 
		rightBracetLTok);

	FST::FST RIFKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		RIFKWTok);

	FST::FST endL(
		lexem,                                                                                                        // ципочка распознавания 
		endKWTok);

	FST::FST commaL(
		lexem,                                                                                                        // ципочка распознавания 
		commaLTok);

	FST::FST semicolonL(
		lexem,                                                                                                        // ципочка распознавания 
		semicolonLTok);

	FST::FST plusL(
		lexem,                                                                                                        // ципочка распознавания 
		plusLTok);

	FST::FST minusL(
		lexem,                                                                                                        // ципочка распознавания 
		minusLTok);

	FST::FST multiplicationL(
		lexem,                                                                                                        // ципочка распознавания 
		multiplicationLTok);

	FST::FST devisionL(
		lexem,                                                                                                        // ципочка распознавания 
		devisionLTok);

	FST::FST equalL(
		lexem,                                                                                                        // ципочка распознавания 
		equalLTok);

	FST::FST boolequalL(
		lexem,                                                                                                        // ципочка распознавания 
		boolequalLTok);

	FST::FST lesseqL(
		lexem,                                                                                                        // ципочка распознавания 
		lesseqLTok);

	FST::FST moreeqL(
		lexem,                                                                                                        // ципочка распознавания 
		moreeqLTok);

	FST::FST lessL(
		lexem,                                                                                                        // ципочка распознавания 
		lessLTok);

	FST::FST moreL(
		lexem,                                                                                                        // ципочка распознавания 
		moreLTok);

	FST::FST IntTKeyWord(
		lexem,
		IntTKWTOk);

	FST::FST StrTKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		StrTKWTok);

	FST::FST BoolTKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		BoolTKWTok);

	FST::FST defFKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		defFKWTok);

	FST::FST defKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		defKWTok);

	FST::FST retKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		retKWTok);

	FST::FST IFKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		IFKWTok);

	FST::FST putKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		putKWTok);

	FST::FST trueKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		trueKWTok);

	FST::FST falseKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		falseKWTok);

	FST::FST StartPointKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		StartPointKWTok);

	FST::FST identificator(
		lexem,                                                                                                        // ципочка распознавания 
		identificatorTok);

	FST::FST IntTLiteral(
		lexem,
		IntTLiteralTok);

	FST::FST StrTLiteral(
		lexem,
		StrTLiteralTok);

	FST::FST newlineKeyWord(
		lexem,                                                                                                        // ципочка распознавания 
		newlinelLTok);

#endif // TOKENINIT	

	FST::FST tokenLexems[NUMLEXEM] =
		{ StartPointKeyWord, StrTKeyWord, IntTKeyWord, BoolTKeyWord, StrTLiteral, IntTLiteral , defFKeyWord, moreL,
		defKeyWord, retKeyWord, putKeyWord, trueKeyWord, falseKeyWord, endL, leftBracetL, rightBracetL,
		identificator, commaL, semicolonL, plusL, minusL, multiplicationL, lessL, boolequalL,
		devisionL, equalL, newlineKeyWord, IFKeyWord, RIFKeyWord, moreeqL, lesseqL};

	char shortLexm[NUMLEXEM] = 
		{ LEX_ID, LEX_STRT, LEX_INTT, LEX_BOOLT, LEX_LITERAL, LEX_LITERAL, LEX_DEFF, LEX_MORE,
		 LEX_DEF, LEX_RET, LEX_PUT, LEX_LITERAL, LEX_LITERAL, LEX_END, LEX_LEFTHESIS, LEX_RIGHTHESIS,
		LEX_ID, LEX_COMMA, LEX_SEMICOLON, LEX_PLUS, LEX_MINUS, LEX_STAR, LEX_LESS, LEX_BOOLEAQUl,
		LEX_DIRSLASH, LEX_EQUAl, LEX_NEWLINE, LEX_IF, LEX_RIF, LEX_MOREEQ, LEX_LESSEQ};

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
		// чтение между кавычками
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
			// уборка лишних пробелов после сепараторов
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
			// уборка лишних пробелов перед сепараторами
			if (isSymInStr(str[i], spaceSym) && isSymInStr(str[i + 1], separators))
			{
				continue;
			}

			resStr += str[i];
			// уборка лишних пробелов перед сепараторами
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

			// обработка не пробельных разделителей
			if (i != orifginalStr.length() && !isSymInStr(orifginalStr[i], spaceAndNewlineSym))
			{
				char lexemShrt[2] = { orifginalStr[i], '\0' };
				char lexemLng[3] = { orifginalStr[i], orifginalStr[i + 1],  '\0' };

				parsingLexem(
					isSymInStr(orifginalStr[i], moreLessEqual) && orifginalStr[i + 1] == LEX_EQUAl ? i++, lexemLng : lexemShrt,
					lexTable, idTable, entryL, entryI, lexTable.positions[lexCounter++].line, litCounter, lastToken, preLastToken, lexCounter);

				if (orifginalStr[i] == LEX_EQUAl)
					lexTable.posLEX_EQUALS[lexTable.posLEX_EQUALSSize++] = lexTable.size - 1;

				//// обработка односимвольных разделителей
				//if (orifginalStr[i + 1] != LEX_EQUAl)
				//{
				//	parsingLexem(lexemShrt, lexTable, idTable, entryL, entryI, lexTable.positions[lexCounter++].line, litCounter, lastToken, preLastToken, lexCounter);

				//	if (orifginalStr[i] == LEX_EQUAl)
				//		lexTable.posLEX_EQUALS[lexTable.posLEX_EQUALSSize++] = lexTable.size - 1;
				//}
				//// обработка разделителей соостоящих из 2ух символов
				//else if (isSymInStr(orifginalStr[i], moreLessEqual))
				//{
				//	parsingLexem(lexemLng, lexTable, idTable, entryL, entryI, lexTable.positions[lexCounter++].line, litCounter, lastToken, preLastToken, lexCounter);
				//	i++;
				//}				
			}

			//сброс конца строки
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
		// считывание лексемы
		else
		{
			// чтение между кавычек
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
		lexTable.table[i].lexema[0] != LEX_END &&																  // правая фигурная
		i < lexTable.size &&																						  // конец LT
		!(lexTable.table[i].lexema[0] == LEX_RIGHTHESIS && lexTable.table[i + 1].lexema[0] == LEX_SEMICOLON);		  // конец объявления прототипа F
		i++, aE = i)
	{
		LT::Entry elLT = lexTable.table[i];
		// обработка вложенной области видимости 
		if (elLT.lexema[0] == LEX_ID && lexTable.table[i - 2].lexema[0] == LEX_DEFF || elLT.lexema[0] == LEX_STARTPOINT)
		{
			set_aB(lexTable, idTable, lstChngId, i, aE);

			set_aE(idTable, lstChngId, i, aE);
		}
		else
		{
			if (elLT.lexema[0] == LEX_ID && idTable.table[elLT.idxTI].vsbAr.aB == NOVSBAR)
			{ 
				idTable.table[elLT.idxTI].vsbAr.aB = aB;
				lstChngId = elLT.idxTI;
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
	return (a.aB > b.aB && a.aE < b.aE) || (a.aB == b.aB && a.aE == b.aE);
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
		if (elLT.lexema[0] == LEX_ID && lexTable.table[i - 2].lexema[0] == LEX_DEFF || elLT.lexema[0] == LEX_STARTPOINT)
		{
			int aB = i;
			set_aB(lexTable, idTable, lasatChangedIdentificator, aB, i);

			set_aE(idTable, lasatChangedIdentificator, aB, i);
		}
	}

	// проверка на ошибку переопределения
	for (int i = 0; i < idTable.size; i++)
	{
		IT::Entry elIT = idTable.table[i];
		for (int e = i - 1; e >= 0; e--)
		{
			if (!(elIT.vsbAr != idTable.table[e].vsbAr) && elIT.idtype == idTable.table[e].idtype)
			{
				if (strcamper(elIT.id, idTable.table[e].id) && elIT.vsbAr <= idTable.table[e].vsbAr)
				{
					printf("переопределение идентификатора: %s,\t{%d, %d}\n", elIT.id, lexTable.positions[elIT.idxfirstLE - 1].line, lexTable.positions[elIT.idxfirstLE - 1].colone);
				}
			}
		}

	}

}
void ChangeLTWithPN(LT::LexTable& lexTable, IT::IdTable& idTable)
{
	// обработка вырадений в польскую нотацию
	int countEmptyEl = 0;
	for (int i = 0; i < lexTable.posLEX_EQUALSSize; i++)
	{
		PN::PolishNotation(lexTable.posLEX_EQUALS[i], lexTable, idTable, countEmptyEl);
	}
	// исключение пустых элементов
	LT::Entry* cleanTable = new  LT::Entry[lexTable.size - countEmptyEl];
	for (int i = 0, e = 0; i < lexTable.size; i++)
	{
		if (lexTable.table[i].lexema[0] != EMPTYSTR)
		{
			cleanTable[e++] = lexTable.table[i];
		}
		else
		{
			continue;
		}
	}
	// очистка памяти
	delete[] lexTable.table;
	lexTable.table = cleanTable;	
	lexTable.size = lexTable.size - countEmptyEl;
}
