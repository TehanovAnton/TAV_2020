#include"TAV_2020_standartLibary.h"
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
}