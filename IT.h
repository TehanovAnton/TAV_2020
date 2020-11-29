#pragma once
#define ID_MAXSIZE 5                     // максимальное количество символов в идентификаторе
#define TI_MAXSIZE 4096                  // максимальное количество строк в таблице идентификатороф
#define TI_INTT_DEFAULT 0x00000000       // значение по умолчанию дл€ типа IntT
#define TI_STRT_DEFAULT 0x00             // значение по умолчанию дл€ типа StrT
#define TI_TRUE_VAL 1                // значение по умолчанию дл€ true
#define TI_FALSE_VAl 0                // значение по умолчанию дл€ fasle, значение по умолчанию дл€ BoolT
#define TI_NULLIDX 0xfffffffff           // нет элемента в таблице идентификатора 
#define TI_STR_MAXSIZE 255
#define NOVSBAR -1
#include "LT.h"
#include<iostream>
#include "Error.h"

namespace IT
{
    enum IDDATATYPE                     // типы данных идентификаторов: IntT, StrT, BoolT
    {
        INT = 1, STR = 2, BOOL = 3
    };

    enum IDTYPE
    {
        V = 1, F = 2, P = 3, L = 4      // типы иеднтификаторов: переменна€, функци€, параметр, литерал
    };

    struct visibleArea
    {
        int aB;
        int aE;

        visibleArea();
    };

    struct Entry                        // строка таблийцы идентификаторов 
    {
        int idxfirstLE;                 // индекс строки в таблице лексем
        char id[ID_MAXSIZE];            // идентификато(автоматически усекаетс€ до ID_MAXSIZE)
        IDDATATYPE iddatatype;          // тип данных
        IDTYPE idtype;                  // тип идентификаторов
        union
        {
            int vint;                               // значение IntT
            bool vbool;                             // значение BoolT
            struct VStrT                                  // значение StrT
            {
                char len;                           // количество символо в string
                char str[TI_STR_MAXSIZE - 1];       // символы string
            }   vste[TI_STR_MAXSIZE];               // значение string

        } values;                                   // значение идентификатора
        visibleArea vsbAr;

        Entry();
        // заполнение IntT и BoolT lit
        Entry(int vintbool, int lexCounter, int& litCounter, IDDATATYPE piddatatype, IDTYPE pidtype);
        // заполнение StrT lit
        Entry(char* vstr, int lexCounter, int& litCounter, IDDATATYPE piddatatype, IDTYPE pidtype);
        // заполнение параметра, переменной, имени функции
        Entry(char* lexem, int lexCounter, IDDATATYPE piddatatype, IDTYPE pidtype);
    };

    struct IdTable
    {
        int maxsize;                                // Ємкость таблицы идентификаторов < TI_MAXSIZE
        int size;                                   // текущий таблицы идентификаторов < maxsize
        Entry* table;                               // массив строк таблицы идентификаторов

        void PrintTable();
    };

    IdTable Create(
        int size // < TI_MAXSIZE
    );

    void Add(
        IdTable& idtable,
        Entry& entry,
        int pos_line,
        int pos_colone
    );

    Entry Getentry(                     // получить строку таблицы идентификаторов
        IdTable& idtable,               // экземпл€р таблицы идентификаторов
        int n                           // номер вощвращаемой строки
    );

    int Istd(                           // возврат номера строки(еслти есть) или TI_NULLIDX
        IdTable& idtable,               // экземпл€р таблицы идентификаторов
        char id[ID_MAXSIZE]             // идентификаторов
    );

    int IstdStrTLitByValue(IdTable idTable, char value[]);

    int IstdBoolTLitByValue(IdTable idTable, bool value);

    int IstdIntTLitByValue(IdTable idTable, int value);

    int IstdByID(IdTable idTable, char value[]);

    int IstdFromCurrent(IdTable& idtable, char* id);

    void Delete(
        IdTable& idtable
    );
}