#pragma once
#define ID_MAXSIZE 5                     // ������������ ���������� �������� � ��������������
#define TI_MAXSIZE 4096                  // ������������ ���������� ����� � ������� ���������������
#define TI_INTT_DEFAULT 0x00000000       // �������� �� ��������� ��� ���� IntT
#define TI_STRT_DEFAULT 0x00             // �������� �� ��������� ��� ���� StrT
#define TI_TRUE_VAL 1                // �������� �� ��������� ��� true
#define TI_FALSE_VAl 0                // �������� �� ��������� ��� fasle, �������� �� ��������� ��� BoolT
#define TI_NULLIDX 0xfffffffff           // ��� �������� � ������� �������������� 
#define TI_STR_MAXSIZE 255
#define NOVSBAR -1
#include "LT.h"
#include<iostream>
#include "Error.h"

namespace IT
{
    enum IDDATATYPE                     // ���� ������ ���������������: IntT, StrT, BoolT
    {
        INT = 1, STR = 2, BOOL = 3
    };

    enum IDTYPE
    {
        V = 1, F = 2, P = 3, L = 4      // ���� ���������������: ����������, �������, ��������, �������
    };

    struct visibleArea
    {
        int aB;
        int aE;

        visibleArea();
    };

    struct Entry                        // ������ �������� ��������������� 
    {
        int idxfirstLE;                 // ������ ������ � ������� ������
        char id[ID_MAXSIZE];            // ������������(������������� ��������� �� ID_MAXSIZE)
        IDDATATYPE iddatatype;          // ��� ������
        IDTYPE idtype;                  // ��� ���������������
        union
        {
            int vint;                               // �������� IntT
            bool vbool;                             // �������� BoolT
            struct VStrT                                  // �������� StrT
            {
                char len;                           // ���������� ������� � string
                char str[TI_STR_MAXSIZE - 1];       // ������� string
            }   vste[TI_STR_MAXSIZE];               // �������� string

        } values;                                   // �������� ��������������
        visibleArea vsbAr;

        Entry();
        // ���������� IntT � BoolT lit
        Entry(int vintbool, int lexCounter, int& litCounter, IDDATATYPE piddatatype, IDTYPE pidtype);
        // ���������� StrT lit
        Entry(char* vstr, int lexCounter, int& litCounter, IDDATATYPE piddatatype, IDTYPE pidtype);
        // ���������� ���������, ����������, ����� �������
        Entry(char* lexem, int lexCounter, IDDATATYPE piddatatype, IDTYPE pidtype);
    };

    struct IdTable
    {
        int maxsize;                                // ������� ������� ��������������� < TI_MAXSIZE
        int size;                                   // ������� ������� ��������������� < maxsize
        Entry* table;                               // ������ ����� ������� ���������������

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

    Entry Getentry(                     // �������� ������ ������� ���������������
        IdTable& idtable,               // ��������� ������� ���������������
        int n                           // ����� ������������ ������
    );

    int Istd(                           // ������� ������ ������(����� ����) ��� TI_NULLIDX
        IdTable& idtable,               // ��������� ������� ���������������
        char id[ID_MAXSIZE]             // ���������������
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