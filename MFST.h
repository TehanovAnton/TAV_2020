#pragma once
#include<iostream>
#include<iomanip>
#include<stack>
#include"GRB.h"
#include"LT.h"
#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

#define MFST_TRACE_START std::cout << std::setw(4) << std::left << "Шаг" << ":"\
	<< std::setw(20) << std::left << "Правило" \
	<< std::setw(30) << std:: left << "Входная лента" \
	<< std::setw(20) << std::left << " Creek" \
	<< std::endl;

#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)

#define MFST_TRACE1 std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": " \
	<< std::setw(20) << std::left << rule.getCRule(rbuf,nrulechain) \
	<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
	<< std::setw(20) << std::left << getCSt(sbuf) \
	<< std::endl;

#define MFST_TRACE2 std::cout << std::setw(4) << std::left << FST_TRACE_n << ": " \
	<< std::setw(20) << std::left << " " \
	<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
	<< std::setw(20) << std::left << getCSt(sbuf) \
	<< std::endl;

#define MFST_TRACE3 std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": "	\
					<< std::setw(20) << std::left << " " \
					<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
					<< std::setw(20) << std::left << getCSt(sbuf) \
					<< std::endl;

#define MFST_TRACE4(c) std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": " << std::setw(20) << std::left << c << std::endl;
#define MFST_TRACE5(c) std::cout << std::setw(4) << std::left << FST_TRACE_n << ": " << std::setw(20) << std::left << c << std::endl;
#define MFST_TRACE6(c,k) std::cout << std::setw(4) << std::left << FST_TRACE_n << ": " << std::setw(20) << std:: left << c << k << std::endl;
#define MFST_TRACE7 std::cout << std::setw(4) << std::left << state.lenta_position << ": "\
	<< std::setw(20) << std::left << rule.getCRule(rbuf, state.nrulechain) \
	<< std::endl;

typedef std::stack<short> MFSTSTSTACK;									 // стек автомата

namespace MFST			   
{
	
	struct MfstState													 // состояние автомата (для сохранения)
	{
		short lenta_position;											 // позиция на ленте
		short nrule;													 // номер текущего правила
		short nrulechain;												 // номер текущей цепочки
		MFSTSTSTACK st;													 // стек автомата
		MfstState();													 
		MfstState(
			short pposition,
			MFSTSTSTACK pst,			  
			short pnrulechain
		);
		MfstState(
			short pposition,
			MFSTSTSTACK pst,
			short pnrule,
			short pnrulechain
		);
	};

	struct Mfst														  // магазинный автомат
	{
		enum RC_STEP												  // код возврата функции step()
		{												  
			NS_OK,													  // найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,												  // не найдено правило грамматики (ошибка в граматике)
			NS_NORULECHAIN,											  // не найдена подходящая цепока правила (ошибка в исходном коде)
			NS_ERROR,												  // неизвестный нетерминальный символ грамматики
			TS_OK,													  // тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,													  // тек. символ ленты != вершине стека, востановленно состояние
			LENTA_END,												  // тек. позиция ленты >= lenta_size
			SURPRISE												  // неожиданный код возврата (ошибка в step())
		};

		struct MfstDiagnosis										  // диагностика
		{
			short lenta_position;
			RC_STEP rc_step;
			short nrule;
			short nrule_chain;
			MfstDiagnosis();

			MfstDiagnosis(
				short plenta_position,
				RC_STEP prc_step,
				short  pnrule,
				short pnrule_chain
			);

		} diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* lenta;											   // перекодированная (TS/NS) лента из (Lextable)
		short lenta_position;										   
		short nrule;
		short nrulechain;
		short lenta_size;
		GRB::Greibach grebach;											// граматика грейбах
		LT::LexTable lex;
		MFSTSTSTACK st;													// стек автоматв
		std::stack<MfstState> storestate;								// стек для сохранения состояний
		Mfst();

		Mfst(
			LT::LexTable plex,
			GRB::Greibach pgrebach
		);

		char* getCSt(char* buf);										// получить содержимое стека

		char* getCLenta(char* buf, short pos, short n = 25);			// лента: n символов начинаая с poc

		char* getDiagnosis(short n, char* buf);						    // получить nую строку диагностики или 0x00

		bool savestate();												// созранить состояние автомата

		bool resstate();												// востановить состояние автомата

		bool push_chain(
			GRB::Rule::Chain chain									    // поместить цепочку правил в стек
		);

		RC_STEP step();												    // выполнит шаг автомата

		bool start();													// запустить автомат

		bool savediagnosis(												// код завершения автомата
			RC_STEP pprc_step
		);

		void printrules();												// вывести последовательность правил

		struct Deducation												// вывод
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation()
			{
				size = 0;
				nrules = 0;
				nrulechains = 0;
			}
		} deducation;						

		bool savededucation();		
	};

	int MFSTSTSTACK_iterate(MFSTSTSTACK st, int idx);

	MfstState MfstState_iterate(std::stack<MfstState> st, int idx);
}