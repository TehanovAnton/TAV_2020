#pragma once
#include<iostream>
extern "C"
{
	int _stdcall strlength(const char* str);
	char* _stdcall substr(const char* str, int begin, int end);
	bool _stdcall strcompare(const char* str1, const char* str2);
}