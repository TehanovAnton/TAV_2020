#include "Error.h"
namespace Error
{
	// ����� ������:	0 - 99	��������� ������
	//				100 - 109	������ ����������
	//				110 - 119	������ �������� � ������ ������ 
	ERROR errors[ERROR_MAX_ENTRY] = // ������� ������
	{
		ERROR_ENTRY(0, "������������ ��� ������"),	//��� ������ ��� ��������� 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "��������� ����"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100, "�������� -in, ������ ���� �����"),
		ERROR_ENTRY(101, ""),
		ERROR_ENTRY(102, "�������� -in, ������ ���� �����"),
		ERROR_ENTRY(103, "������ �������� ����� ��������� ����� (-in)"),
		ERROR_ENTRY(104, "������ ��� �������� ����� ��������� (-log)"),
		ERROR_ENTRY(105, "������ ��� �������� ����� ��������� (-log)"),
		ERROR_ENTRY(106, ""),
		ERROR_ENTRY(107, ""),
		ERROR_ENTRY(108, ""),
		ERROR_ENTRY(109, ""),

		ERROR_ENTRY(110, ""),  													// ������ ������������ �������
		ERROR_ENTRY(111, "������������ ������ � �������� ����� (-in)"),
		ERROR_ENTRY(112, ""),
		ERROR_ENTRY(113, "������������ ������� ������"),
		ERROR_ENTRY(114, "������������ ������� ���������������"),
		ERROR_ENTRY(115, "������������� �������"),
		ERROR_ENTRY(116, "������������� �������������"),
		ERROR_ENTRY(117, "��������������� main"),

		ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
															 
		ERROR_ENTRY(120, "�������������� ����� ��������� � ���������"),			 // ������ �����
		ERROR_ENTRY(121, "�������������� ����� �������� � ���������"),
		ERROR_ENTRY(122, "�������������� ���� ����������� ��������"),
		ERROR_ENTRY(123, "�������������� ���� ��������� ��� ������ �������"),
		ERROR_ENTRY(124, "�������� ���������� ���������� ��� ������ �������"),
		ERROR_ENTRY(125, ""),
		ERROR_ENTRY(126, ""),
		ERROR_ENTRY(127, ""),
		ERROR_ENTRY(128, ""),
		ERROR_ENTRY(129, ""),

		ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),

		ERROR_ENTRY(600, "�������� ��������� ���������"),					// ������ ����������
		ERROR_ENTRY(601, "��������� ��������"),
		ERROR_ENTRY(602, "������ � ���������"),
		ERROR_ENTRY(603, "������ � ����������� ������"),
		ERROR_ENTRY(604, "������ � ����������� ���������� ������"),

		ERROR_ENTRY_NODEF(605), ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(608), ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),

		ERROR_ENTRY(700, "�������������� �������� Semantics.cpp"),					// ������ ������ �����������
		ERROR_ENTRY(701, ""),
		ERROR_ENTRY(702, ""),
		ERROR_ENTRY(703, ""),
		ERROR_ENTRY(704, ""),

		ERROR_ENTRY_NODEF10(800), ERROR_ENTRY_NODEF10(900)
	};

	ERROR geterror(int id)
	{
		bool rangeId = (id > 0 && id < ERROR_MAX_ENTRY) ? true : false;
		if (rangeId)
		{
			for (size_t i = 0; i < ERROR_MAX_ENTRY; i++)
				if (id == errors[i].id || -id == errors[i].id)
				{
					return errors[i];
				}
		}
		else
		{
			return errors[0];
		}
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		bool rangeId = (id > 0 && id < ERROR_MAX_ENTRY) ? true : false;
		if (rangeId)
		{
			for (size_t i = 0; i < ERROR_MAX_ENTRY; i++)
				if (id == errors[i].id || -id == errors[i].id)
				{
					errors[i].intext.col = col;
					errors[i].intext.line = line;
					return errors[i];
				}
		}
		else
		{
			return errors[0];
		}
	}
}
