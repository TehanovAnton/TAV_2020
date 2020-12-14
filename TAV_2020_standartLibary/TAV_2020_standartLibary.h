#pragma once
#include<iostream>
extern "C"
{
	int _stdcall compare(const char* str1, const char* str2);
	char* _stdcall concat(const char* str1, const char* str2);
}