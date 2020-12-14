#include "IT.h"
namespace IT
{
	visibleArea::visibleArea()
	{
		aB = aE = NOVSBAR;
	}

	IdTable Create(int size)
	{
		IdTable lextable = { size, 0, new Entry[size] };
		return lextable;
	}

	Entry::Entry()
	{

	}

	Entry::Entry(char intlit[], int lexCounter, int& litCounter, IDDATATYPE piddatatype, IDTYPE pidtype, char plitNotation)
	{
		// имя литерала
		char buffer[1000];
		_itoa_s(++litCounter, buffer, 1000, 10);
		id[0] = 'L';
		strcopy(id + 1, buffer);

		litТNotation = plitNotation;

		// тип данных; определяется автомату который сработал для предидущей лексемы
		iddatatype = piddatatype;

		// тип идентификатора
		idtype = pidtype;

		// ссылка на LT
		idxfirstLE = lexCounter - 1;

		strcopy(values.vint, intlit);
	}

	Entry::Entry(int vintbool, int lexCounter, int& litCounter, IDDATATYPE piddatatype, IDTYPE pidtype)
	{
		// имя литерала
		char buffer[1000];
		_itoa_s(++litCounter, buffer, 1000, 10);
		id[0] = 'L';
		strcopy(id + 1, buffer);

		litТNotation = '\0';

		// тип данных; определяется по автомату который сработал для предидущей лексемы
		iddatatype = piddatatype;

		// тип идентификатора
		idtype = pidtype;

		// ссылка на LT
		idxfirstLE = lexCounter - 1;

		values.vbool = (bool)vintbool;
	}

	Entry::Entry(char* vstr, int lexCounter, int& litCounter, IDDATATYPE piddatatype, IDTYPE pidtype)
	{
		// имя литерала
		char buffer[1000];
		_itoa_s(++litCounter, buffer, 1000, 10);
		id[0] = 'L';

		strcopy(id + 1, buffer);

		// тип данных; определяется автомату который сработал для предидущей лексемы
		iddatatype = piddatatype;

		// тип идентификатора
		idtype = pidtype;

		// ссылка на LT
		idxfirstLE = lexCounter - 1;

		litТNotation = '\0';

		// значение по умолчанию
		strcopy(values.vste->str, vstr);
	}

	Entry::Entry(char* lexem, int lexCounter, IDDATATYPE piddatatype, IDTYPE pidtype)
	{
		// имя параметра
		memset(id, 0, sizeof(char) * ID_MAXSIZE);
		for (size_t i = 0; i < strlen(lexem) && i < ID_MAXSIZE; i++)
		{
			id[i] = lexem[i];
			if (i + 1 == ID_MAXSIZE)
				id[i + 1] = '\0';
		}

		// тип данных; определяется автомату который сработал для предидущей лексемы
		iddatatype = piddatatype;

		// тип идентификатора
		idtype = pidtype;

		// ссылка на LT
		idxfirstLE = lexCounter - 1;

		litТNotation = '\0';

		// значение по умолчанию
		if (iddatatype == IT::INT)
			values.vint = 0;
		else
		{
			values.vste->len = 0;
			values.vste->str[0] = '\0';
		}
	}

	void Add(IdTable& idtable, Entry& entryI, int pos_line, int pos_colone)
	{
		if (idtable.size == idtable.maxsize)
		{
			throw ERROR_THROW_IN(114, pos_line, pos_colone)
		}
		else
		{
			idtable.table[idtable.size] = entryI;
			idtable.size++;
			entryI = IT::Entry();
		}
	}

	Entry Getentry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int IstdIntTLitByValue(IdTable idTable, char* value)
	{
		for (size_t i = 0; i < idTable.size; i++)
		{
			if (idTable.table[i].idtype == IT::L && idTable.table[i].iddatatype == IT::INT && strcmp(value, idTable.table[i].values.vint) == 0)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}
	int IstdBoolTLitByValue(IdTable idTable, bool value)
	{
		for (size_t i = 0; i < idTable.size; i++)
		{
			if (idTable.table[i].idtype == IT::L && idTable.table[i].iddatatype == IT::BOOL && value == idTable.table[i].values.vbool)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}
	int IstdStrTLitByValue(IdTable idTable, char value[])
	{
		for (size_t i = 0; i < idTable.size; i++)
		{
			if (idTable.table[i].idtype == IT::L && idTable.table[i].iddatatype == IT::STR)
			{
				bool res = true;
				// сравнение значений по символам 
				for (size_t e = 0; e < strlen(value) && res; e++)
				{
					res &= idTable.table[i].values.vste->str[e] == value[e];
				}

				if (res)
				{
					return i;
				}
			}
		}
		return TI_NULLIDX;
	}
	int IstdByID(IdTable idTable, char value[])
	{
		for (size_t i = 0; i < idTable.size; i++)
		{
			bool res = true;
			// сравнение значений по символам 
			size_t e = 0;
			for (; e < strlen(value) && res; e++)
			{
				res &= idTable.table[i].id[e] == value[e];
			}

			if (res && e == strlen(idTable.table[i].id))
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}
	int IstdFromCurrent(IdTable& idtable, char* id)
	{
		for (int i = idtable.size - 1; i >= 0; i--)
		{
			bool res = true;
			// сравнение строк
			for (size_t e = 0; e < strlen(id) && e < ID_MAXSIZE && res; e++)
			{
				res &= idtable.table[i].id[e] == id[e];
			}

			if (res && strlen(id) == strlen(idtable.table[i].id))
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idTable)
	{
		delete& idTable;
	}

	void IdTable::PrintTable()
	{
		std::cout << "\n\n";
		for (size_t i = 0; i < this->size; i++)
		{
			if (this->table[i].iddatatype == IT::INT)
			{
				std::printf("id: %s, value: %s, idxfirstLE: %d, datatype: INT\n", this->table[i].id, this->table[i].values.vint, this->table[i].idxfirstLE);
			}
			else if (this->table[i].iddatatype == IT::BOOL)
			{
				std::printf("id: %s, value: %d, idxfirstLE: %d, datatype: BOOL\n", this->table[i].id, this->table[i].values.vbool, this->table[i].idxfirstLE);
			}
			else
				std::printf("id: %s, value: %s, idxfirstLE: %d, datatype: STR;\n", this->table[i].id, this->table[i].values.vste->str, this->table[i].idxfirstLE);			//std::cout << this->table[i].id << " = " << this->table[i].values.vste->str << "; " << this->table[i].idxfirstLE;

			std::cout << '\n';
		}
	}
}