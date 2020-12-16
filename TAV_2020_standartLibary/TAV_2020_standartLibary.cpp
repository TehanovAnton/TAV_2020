#include"TAV_2020_standartLibary.h"
#include<stdio.h>
extern "C"
{
	int _stdcall strca(char* str1, char* str2)
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

	int _stdcall strle(char* str)
	{
		return strlen(str);
	}

	char* _stdcall strsu(int e, int b, char* str)
	{
		char* buffer = reinterpret_cast<char*>(calloc(e - b + 2, sizeof(char)));

		if (buffer == nullptr && (b > e || b < 0 || e < 0))
		{
			buffer = reinterpret_cast<char*>(calloc(1, sizeof(char)));
			buffer[0] = '\0';
			return buffer;
		}

		int k = 0;
		for (int i = b; i <= e && i < strlen(str); ++i, ++k)
			buffer[k] = str[i];

		buffer[k] = '\0';
		return buffer;
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