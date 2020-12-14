#include"TAV_2020_standartLibary.h"
#include<stdio.h>
extern "C"
{
	int _stdcall compa(char* str1, char* str2)
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

	void _stdcall printCons(char* str)
	{
		std::cout << str << "\n";
	}

	char* _stdcall conca(char* str1, char* str2)
	{
		char* buffer = reinterpret_cast<char*>(calloc(strlen(str1) + strlen(str2) + 1, sizeof(char)));

		if (buffer == nullptr)
			return 0;

		for (int i = 0; str1[i]; ++i)
			buffer[i] = str1[i];

		for (int i = 0; str2[i]; ++i)
			buffer[i + strlen(str1)] = str2[i];

		return buffer;
	}
}