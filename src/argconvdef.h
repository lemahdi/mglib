/*
 * Copyright			: 2010 by MG
 * File					: argconvdef.h
 * Version				: 0.1.17
 * Last changed			: 25 DEC 2010
 * Purpose				: Specializations of MG_ArgConv  and MG_ArgConvReverse
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "argconv.h"


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


MG_NAMESPACE_END
