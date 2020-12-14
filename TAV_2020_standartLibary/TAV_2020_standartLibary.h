#pragma once
#include<iostream>
extern "C"
{
	int _stdcall compa(const char* str1, const char* str2);
	char* _stdcall conca(const char* str1, const char* str2);
}