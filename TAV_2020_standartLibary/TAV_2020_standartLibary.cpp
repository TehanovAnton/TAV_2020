#include"TAV_2020_standartLibary.h"
#include<stdio.h>
extern "C"
{
	int _stdcall strlength(const char* str)
	{
		return std::strlen(str);
	}

	char* _stdcall substr(const char* str, int begin, int end)
	{
		int lenth = strlen(str);
		if (lenth != 0 && lenth -1 >= begin && begin >= 0 && end >= begin && end >= 0)
		{
			char* res = new char[end - begin + 1];
			int e = 0;
			for (int i = begin; i <= end && i < lenth; i++, e++)
				res[e] = str[i];
			res[e] = '\0';
			return res;
		}
		else
			return new char[] {""};
	}

	bool _stdcall strcompare(const char* str1, const char* str2)
	{
		bool res = false;
		int len1 = strlen(str1);
		if (len1 == strlen(str2) && len1 > 0)
		{
			res = true;
			for (int i = 0; i < len1 && res; i++)
				res &= str1[i] == str2[i];
		}

		return res;
	}

	/*_asm
	{	.code
		printconsole PROTO : DWORD, : DWORD
		int_to_char PROTO : DWORD, : SDWORD

		int_to_char PROC uses eax ebx ecx edi esi,
		pstr : dword, ; адрес строки возврата
		intfield : sdword; число для преобразования
		mov edi, pstr
		mov esi, 0
		mov eax, intfield
		cdq
		mov ebx, 10
		idiv ebx
		test eax, 80000000h
		jz plus
		neg eax
		neg edx
		mov cl, '-'
		mov[edi], cl
		inc edi
		plus :
		push dx
			inc esi
			test eax, eax
			jz fin
			cdq
			idiv ebx
			jmp plus
			fin :
		mov ecx, esi
			write :
		pop bx
			add bl, '0'
			mov[edi], bl
			inc edi
			loop write
			push offset pstr
			pop eax
			ret
			int_to_char ENDP

			printconsole PROC uses eax ebx ecx edi esi,
			pstr :dword,
			ptitle : dword

			push ptitle
			call SetConsoleTitleA

			push - 11
			call GetStdHandle

			mov esi, pstr
			mov edi, -1

			count:
		inc edi
			cmp byte ptr[esi + edi], 0
			jne count

			push 0
			push 0
			push edi
			push pstr
			push eax
			call WriteConsoleA

			ret
			printconsole ENDP
			end
	}*/
}