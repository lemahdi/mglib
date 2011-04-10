#pragma once


#include "argconv.h"


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
