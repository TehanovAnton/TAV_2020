#include "LT.h"

bool isSymInStr(char sym, const char str[])
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (sym == str[i]) return true;
		else continue;
	}
	return false;
}

namespace LT
{

	Entry::Entry()
	{
		idxTI = TI_NULLIDX;
		lexema;
		sn = 1;
		cn = 1;
		oper_v = '\0';
	}

	Entry::Entry(char origlex, char shortLexm, int line, int idxti)
	{
		lexema[0] = shortLexm;
		lexema[1] = '\0';
		oper_v = shortLexm == 'v' ? origlex : '\0';
		sn = line;
		cn = 0;		// определяется в GetLexPositions
		idxTI = idxti;
	}

	Position::Position()
	{
		colone = 0;
		line = 0;
	}

	Position::Position(int ln, int cl)
	{
		line = ln;
		colone = cl;
	}

	LexTable Create(int size)
	{
		LexTable lextable = { size, 0, new Entry[size], 0, new Position[size], 0, new int[size] };
		return lextable;
	}

	void Add(LexTable& lextable, Entry& entry)
	{
		if (lextable.size == lextable.maxsize)
		{
			throw ERROR_THROW_IN(113, lextable.positions[lextable.size].line, lextable.positions[lextable.size].colone)
		}
		else
		{
			lextable.table[lextable.size] = entry;
			// заполнение очереди положений IF, RIF
			if (entry.lexema[0] == LEX_IF || entry.lexema[0] == LEX_RIF)
			{
				lextable.posLEX_IF_RIF.push(lextable.size);
			}
			lextable.size++;

			entry = LT::Entry();
		}
	}

	int IstdByID(LexTable lexTable, char value[])
	{
		for (size_t i = 0; i < lexTable.size; i++)
		{
			bool res = true;
			// сравнение значений по символам 
			size_t e = 0;
			for (; e < strlen(value) && res; e++)
			{
				res &= lexTable.table[i].lexema[e] == value[e];
			}

			if (res && e == strlen(value))
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete& lextable;
	}

	bool ITPointerBefore(LT::LexTable lexTable, int idx, int ITpointer)
	{
		for (int i = idx; i >= 0; i--)
		{
			if (lexTable.table[i].idxTI == ITpointer)
			{
				return true;
			}
		}

		return false;
	}

	void LexTable::GetLexemsPosition(std::string originalText)
	{
		char longLexem[TI_STR_MAXSIZE];

		for (size_t i = 0, e = 0, line = 1, colone = 1; i < originalText.length() + 1; i++, e++, colone++)
		{
			if (isSymInStr(originalText[i], sepSpaNewlSym) || i == originalText.length())
			{
				if (e != 0)
					this->positions[this->posSize++] = Position(line, colone - strlen(longLexem));

				if (i != originalText.length() && !isSymInStr(originalText[i], spaceAndNewlineSym))
				{
					this->positions[this->posSize++] = Position(line, colone);
					if (isSymInStr(originalText[i], moreLessEqual) && originalText[i + 1] == LEX_EQUAl)
						i++, colone++;
				}

				//сброс колонки и учёт строки
				if (originalText[i] == newlSym)
				{
					this->positions[this->posSize++] = Position(line, colone);
					line++;
					colone = 0;
					e = -1;
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
				if (originalText[i] == quotation)
				{
					longLexem[e] = originalText[i]; i++; e++;
					longLexem[e + 1] = '\0';
					while (originalText[i] != quotation)
					{
						longLexem[e] = originalText[i];
						longLexem[e + 1] = '\0';
						i++; e++; colone++;
					}
					longLexem[e] = originalText[i];
					longLexem[e + 1] = '\0';
					colone++;
					continue;
				}
				else
				{
					longLexem[e] = originalText[i];
					longLexem[e + 1] = '\0';
				}
			}
		}
	}

	void LexTable::PrintTable()
	{
		std::cout << this->table[0].sn << '\t';
		for (size_t i = 0; i < this->size; i++)
		{
			if (this->table[i].lexema[0] != '\0')
			{
				if (table[i].lexema[0] == 'v')
					std::cout << table[i].oper_v;
				else
				{
					std::cout << table[i].lexema;
					if (table[i].lexema[0] == ';')
					{
						if (this->table[i].sn != -1 && i + 1 != this->size)
							printf("\t\t\t\t{%d, %d}\n%d\t", this->table[i].sn, this->table[i].cn, this->table[i + 1].sn);
					}

					if (i + 1 == this->size) // коректный выввод последненго элемента
						printf("\t\t\t\t{%d, %d}", this->table[i].sn, this->table[i].cn);
				}
			}
			else
			{
				continue;
			}
		}
	}
}