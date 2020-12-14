#pragma once
#include<iostream>
extern "C"
{
	int _stdcall compa(char* str1, char* str2);
	char* _stdcall conca(char* str1, char* str2);
	void _stdcall printCons(char* str);					
}