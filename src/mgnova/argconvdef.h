/*
 * Copyright			: 2010 by MG
 * File					: argconvdef.h
 * Version				: 0.1.17
 * Last changed			: 25 DEC 2010
 * Purpose				: Specializations of MG_ArgConv  and MG_ArgConvReverse
 * Author				: MM Akkouh
 * Notes				: 3 letters months name
 *						  3 letters currencies name
 */


#pragma once


#include "mgnova/argconv.h"


MG_NAMESPACE_BEGIN


enum MONTHS_3L
{
	JAN,
	FEB,
	MAR,
	APR,
	MAY,
	JUN,
	JUL,
	AUG,
	SEP,
	OCT,
	NOV,
	DEC,

	NB_MONTHS_3L
};

extern const MG_ArgConv			MonthsConvertor;
extern const MG_ArgConvReverse	MonthsRevertor;


enum CURRENCY_NAME
{
	USD,
	EUR,
	JPY,
	GBP,

	NB_CURRENCIES
};

extern const MG_ArgConv			CurrenciesNameConvertor;
extern const MG_ArgConvReverse	CurrenciesNameRevertor;


/* enum for interpolation methods */
enum INTERPOL_METHOD {
	NONE_INTERPOL,			// no interpolation
	STEPUPLEFT_INTERPOL,	// step up left interpol method
	STEPUPRIGHT_INTERPOL,	// step up right interpol method
	LIN_INTERPOL,			// linear interpol method

	NB_INTERPOL_METH
};

extern const MG_ArgConv			InterpolMethodConvertor;
extern const MG_ArgConvReverse	InterpolMethodRevertor;


MG_NAMESPACE_END
