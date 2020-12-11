#include "PolishNotation.h"
#define macros 0

namespace PN
{
	PNstr::PNstr()
	{
		maxsize = TI_STR_MAXSIZE;
		size = 0;
		pnstr = new char[TI_STR_MAXSIZE];
	}

	PNstr::PNstr(int m, int s, char* pns)
	{
		maxsize = m;
		size = s;
		pnstr = pns;
	}

	PNLTstr::PNLTstr()
	{
		maxsize = TI_STR_MAXSIZE;
		size = 0;
		origIdx = new int[TI_STR_MAXSIZE];
		pnstr = new LT::Entry[TI_STR_MAXSIZE];
	}

	PNLTstr::PNLTstr(int m)
	{
		maxsize = m;
		size = 0;
		origIdx = new int[m];
		pnstr = new LT::Entry[m];
	}

	void PNLTstr::Addpnstr(LT::Entry entryL, IT::IdTable& idTable, int indx, int origIdx)
	{
		this->pnstr[indx] = entryL;
		this->origIdx[indx] = origIdx;
	}

	SymWithPrioryty::SymWithPrioryty()
	{
		sym = LEX_PLUS;
		pRioryty = 2;
	}

	SymWithPrioryty::SymWithPrioryty(char s, short p)
	{
		sym = s;
		pRioryty = p;
	}

	int GetPrioryty(char sym, SymWithPrioryty prior[])
	{
		for (size_t i = 0; i < SymWithPriorytyAmount; i++)
		{
			if (sym == prior[i].sym)
				return prior[i].pRioryty;
			else
				continue;
		}
		return 0;
	}

	void parseFunctoPnstrLT(LT::LexTable& lexTable, IT::IdTable& idTable, PNLTstr& res, int& i, std::stack<LT::Entry>& stck,
		SymWithPrioryty prioryties[])
	{
		LT::Entry
			func = lexTable.table[i++],
			lexLT = lexTable.table[++i];
		int parmCounter = 0;
		for (char lex = LT::IsOperation(lexTable.table[i].lexema[0]) ? lexTable.table[i].oper_v : lexTable.table[i].lexema[0]; lexTable.table[i].lexema[0] != LEX_RIGHTHESIS;
			lex = LT::IsOperation(lexTable.table[++i].lexema[0]) ? lexTable.table[i].oper_v : lexTable.table[i].lexema[0], lexLT = lexTable.table[i])
		{
			// операнд
			if (lex == LEX_ID && lexTable.table[i + 1].lexema[0] != LEX_LEFTHESIS || lex == LEX_LITERAL)
			{
				// запись операнда в порядке следования
				res.Addpnstr(lexLT, idTable, res.size++, i);
				parmCounter++;
			}
			// функция
			else if (lexTable.table[i + 1].lexema[0] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				std::stack<LT::Entry> stckintern;
				parseFunctoPnstrLT(lexTable, idTable, res, i, stckintern, prioryties);
			}
			// операция
			else if (isSymInStr(lex, Operations_Brackets))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top().lexema[0] == LEX_LEFTHESIS || lex == LEX_LEFTHESIS)
				{
					stck.push(lexLT);
				}
				// закрывающая скобка
				else if (lex == LEX_RIGHTHESIS)
				{
					// выталкивание операций до открывающей скобки
					for (; stck.top().lexema[0] != LEX_LEFTHESIS; )
					{
						res.Addpnstr(stck.top(), idTable, res.size++, i);
						stck.pop();
					}
					stck.pop();
				}
				// в верщине стека и строки опрерация 
				else if (isSymInStr(lex, Operations) && isSymInStr(stck.top().lexema[0], Operations))
				{
					// операция выталкивает из стека операцию с большим либо равным приоритетом
					if (GetPrioryty(lex, prioryties) <= GetPrioryty(stck.top().lexema[0], prioryties))
					{
						res.Addpnstr(stck.top(), idTable, res.size++, i);
						stck.pop();
						stck.push(lexLT);
					}
					// операция в строке проирететней чем в стеке
					else
						stck.push(lexLT);
				}
			}
			// выталкивание всех операций до опустошения стека
			else if (lex == LEX_COMMA)
			{
				for (; !stck.empty(); )
				{
					res.Addpnstr(stck.top(), idTable, res.size++, i);
					stck.pop();
				}
			}
			else
				continue;
		}
		// выталкивание операций до опустошения стека
		for (; !stck.empty(); )
		{
			res.Addpnstr(stck.top(), idTable, res.size++, i);
			stck.pop();
		}

		// приобразование числа параметров в символ 
		char parmCounterStr[2];
		_itoa_s(parmCounter, parmCounterStr, 10);
		LT::Entry el = LT::Entry('\0', parmCounterStr[0], -1, -1);

		res.Addpnstr(el, idTable, res.size++, -1);
		res.Addpnstr(func, idTable, res.size++, -1);
	}
	void parseArrtoPnstrLT(LT::LexTable& lexTable, IT::IdTable& idTable, PNLTstr& res, int& i, std::stack<LT::Entry>& stck,
		SymWithPrioryty prioryties[])
	{
		char
			lex = lexTable.table[++i].lexema[0] == 'v' ? lexTable.table[i].oper_v : lexTable.table[i].lexema[0];
		int parmCounter = 0;
		for (LT::Entry lexLT = lexTable.table[i]; lexTable.table[i].lexema[0] != LEX_RSQBRACKET;
			lex = LT::IsOperation(lexTable.table[++i].lexema[0]) ? lexTable.table[i].oper_v : lexTable.table[i].lexema[0], lexLT = lexTable.table[i])
		{
			// операнд
			if (lex == LEX_ID || lex == LEX_LITERAL)
			{
				// запись операнда в порядке следования
				res.Addpnstr(lexLT, idTable, res.size++, i);
				parmCounter++;
			}
			// массив 
			else if (lex == LEX_LSQBRACKET)
			{
				std::stack<LT::Entry> stckintern;
				parseArrtoPnstrLT(lexTable, idTable, res, i, stckintern, prioryties);
			}
			// операция
			else if (isSymInStr(lex, Operations_Brackets))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top().lexema[0] == LEX_LSQBRACKET || lex == LEX_LSQBRACKET)
				{
					stck.push(lexLT);
				}
				// закрывающая скобка
				else if (lex == LEX_RSQBRACKET)
				{
					// выталкивание операций до открывающей скобки
					for (; stck.top().lexema[0] != LEX_RSQBRACKET; )
					{
						res.Addpnstr(stck.top(), idTable, res.size++, i);
						stck.pop();
					}
					stck.pop();
				}
				// в верщине стека и строки опрерация 
				else if (isSymInStr(lex, Operations) && isSymInStr(stck.top().lexema[0], Operations))
				{
					// операция выталкивает из стека операцию с большим либо равным приоритетом
					if (GetPrioryty(lex, prioryties) <= GetPrioryty(stck.top().lexema[0], prioryties))
					{
						res.Addpnstr(stck.top(), idTable, res.size++, i);
						stck.pop();
						stck.push(lexLT);
					}
					// операция в строке проирететней чем в стеке
					else
						stck.push(lexLT);
				}
			}
			// выталкивание всех операций до опустошения стека
			else if (lex == LEX_COMMA)
			{
				for (; !stck.empty(); )
				{
					res.Addpnstr(stck.top(), idTable, res.size++, i);
					stck.pop();
				}
			}
			else
				continue;
		}
		for (; !stck.empty(); )
		{
			res.Addpnstr(stck.top(), idTable, res.size++, i);
			stck.pop();
		}

		// приобразование числа параметров в символ 
		char parmCounterStr[2];
		_itoa_s(parmCounter, parmCounterStr, 10);
		LT::Entry el = LT::Entry('\0', parmCounterStr[0], -1, -1);
		LT::Entry elArr = LT::Entry('\0', '@', -1, -1);

		res.Addpnstr(el, idTable, res.size++, -1);
		res.Addpnstr(elArr, idTable, res.size++, -1);
	}
	bool PolishNotation(int expressionStart, LT::LexTable& lexTable, IT::IdTable& idTable, int& countEmptyEl, bool IfRif)
	{
		SymWithPrioryty prioryties[SymWithPriorytyAmount] = { prior_1, prior_2, prior_3 };
		PNLTstr res = PNLTstr();
		std::stack<LT::Entry> stck;

		int i = expressionStart + 1;
		char lex = LT::IsOperation(lexTable.table[i].lexema[0]) ? lexTable.table[i].oper_v : lexTable.table[i].lexema[0];
		for (LT::Entry lexLT = lexTable.table[i];
			lexTable.table[i].lexema[0] != LEX_NEWLINE;
			lex = LT::IsOperation(lexTable.table[++i].lexema[0]) ? lexTable.table[i].oper_v : lexTable.table[i].lexema[0], lexLT = lexTable.table[i])
		{
			// операнд
			if ((lex == LEX_ID && lexTable.table[i + 1].lexema[0] != LEX_LEFTHESIS) || lex == LEX_LITERAL)
			{
				// запись операнда в порядке следования
				res.Addpnstr(lexLT, idTable, res.size++, i);
			}
			// функция, последнее условие обеспечивает попадание операции в стек
			else if (lexTable.table[i + 1].lexema[0] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				std::stack<LT::Entry> stckintern;
				parseFunctoPnstrLT(lexTable, idTable, res, i, stckintern, prioryties);
			}
			// массив 
			else if (lex == LEX_LSQBRACKET)
			{
				std::stack<LT::Entry> stckintern;
				parseArrtoPnstrLT(lexTable, idTable, res, i, stckintern, prioryties);
			}
			// знаки операций или скобки 
			else if (isSymInStr(lex, Operations_Brackets))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top().lexema[0] == LEX_LEFTHESIS || lex == LEX_LEFTHESIS)
				{
					stck.push(lexLT);
				}
				// закрывающая скобка
				else if (lex == LEX_RIGHTHESIS || lex == LEX_RSQBRACKET)
				{
					// выталкивание операций до открывающей скобки
					for (; !stck.empty() && stck.top().lexema[0] != LEX_LEFTHESIS; )
					{
						res.Addpnstr(stck.top(), idTable, res.size++, i);
						stck.pop();
					}
					if (!stck.empty())
						stck.pop();

					if (IfRif) // выход если обрабатывается IF RIF
						break;
				}
				// в верщине стека и строки опрерация 
				else if (isSymInStr(lex, Operations) && isSymInStr(stck.top().oper_v, Operations))
				{
					// операция выталкивает из стека операцию с большим либо равным приоритетом
					if (GetPrioryty(lex, prioryties) <= GetPrioryty(stck.top().oper_v, prioryties))
					{
						res.Addpnstr(stck.top(), idTable, res.size++, i);
						stck.pop();
						stck.push(lexLT);
					}
					else
						stck.push(lexLT);
				}

			}
			// ошибка
			else
			{
				return false;
			}
		}
		// выталкивание операций до опустошения стека
		for (; !stck.empty(); )
		{
			res.Addpnstr(stck.top(), idTable, res.size++, i);
			stck.pop();
		}

		// вставка обработанной части LT 
		for (int i = expressionStart + 1, e = 0; lexTable.table[i].lexema[0] != LEX_NEWLINE; i++, e++)
		{
			if (e < res.size)
			{
				lexTable.positions[i] = lexTable.positions[res.origIdx[e]];
				lexTable.table[i] = res.pnstr[e];

				if (!LT::ITPointerBefore(lexTable, i - 1, lexTable.table[i].idxTI))
				{
					idTable.table[lexTable.table[i].idxTI].idxfirstLE = i;
				}
			}
			else
			{
				if (IfRif)				// выход при обработке IF RIF
					break;
				lexTable.table[i].sn = lexTable.table[i].idxTI = TI_NULLIDX;
				lexTable.table[i].lexema[0] = '\0';
				lexTable.positions[i] = LT::Position(-1, -1);
				countEmptyEl++;
			}
		}

		return true;
	}
}
