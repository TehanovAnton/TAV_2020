#pragma once
#include<iostream>
extern "C"
{
	int _stdcall strca(char* str1, char* str2);
	int _stdcall strle(char* str);
	char* _stdcall strsu(int e, int b, char* str);
	void _stdcall printCons(char* str);		
	char* _stdcall conca(char* str1, char* str2);
}