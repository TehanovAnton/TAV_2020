#include"TAV_2020_standartLibary.h"
#include<stdio.h>
extern "C"
{
	int _stdcall compare(const char* str1, const char* str2)
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

	char* _stdcall concat(const char* str1, const char* str2)
	{
		int str1len = strlen(str1), str2len = strlen(str2);
		if (str1len != 0 && str2len != 0)
		{
			char* res = new char[str1len + str2len + 1];
			int i = 0;
			for (; i < str1len; i++)
			{
				res[i] = str1[i];
			}
			for (int e = 0; e < str2len; i++, e++)
			{
				res[i] = str2[e];
			}
			res[i] = '\0';
			return res;
		}
		return new char[] {""};
	}
}