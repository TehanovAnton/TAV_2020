#pragma once
#define	separator_VertLine '|'
#define separators "<>+-*/=(){};,"
#define spaceAndNewlineSym " \n\t"
#define spaceSym " \t"
#define newlSym '\n'
#define quotation '\''
#define sepSpaNewlSym "<>+-*/=(){};, \n\t"
#define moreLessEqual "<>="
#define moreEqual ">="	// oper_v => 'c'
#define lessEqual "<="	// oper_v => 'g'
#define boolEqual "=="	// oper_v => 'h'
#define EMPTYSTR '\0'		 
#include<iostream>

void strcopy(char* destination, char source[]);