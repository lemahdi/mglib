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
	K_JAN,
	K_FEB,
	K_MAR,
	K_APR,
	K_MAY,
	K_JUN,
	K_JUL,
	K_AUG,
	K_SEP,
	K_OCT,
	K_NOV,
	K_DEC,

	NB_MONTHS_3L
};

extern const MG_ArgConv			MonthsConvertor;
extern const MG_ArgConvReverse	MonthsRevertor;


enum CURRENCY_NAME
{
	K_USD,
	K_EUR,
	K_JPY,
	K_GBP,

	NB_CURRENCIES
};
#define CURRENCY_NAME_DEF K_EUR

extern const MG_ArgConv			CurrenciesNameConvertor;
extern const MG_ArgConvReverse	CurrenciesNameRevertor;


enum CALENDAR_NAME
{
	K_USD_CAL,
	K_EUR_CAL,
	K_JPY_CAL,
	K_GBP_CAL,

	NB_CALENDARS
};
#define CALENDAR_NAME_DEF K_EUR_CAL

extern const MG_ArgConv			CalendarsNameConvertor;
extern const MG_ArgConvReverse	CalendarsNameRevertor;


enum FREQUENCY_NAME
{
	K_DAILY = 365,
	K_WEEKLY = 52,
	K_MONTHLY = 12,
	K_BIMONTHLY = 6,
	K_QUARTERLY = 4,
	K_SEMESTERLY = 2,
	K_ANNUALY = 1,

	NB_FREQUENCIES = 7
};

extern const MG_ArgConv			FrequencyNameConvertor;
extern const MG_ArgConvReverse	FrequencyNameRevertor;


enum DAYCOUNT_NAME
{
	K_ACT,
	K_ACT_360,
	K_ACT_365,
	K_U_30_360,
	K_E_30_360,
	K_BUS_252,

	NB_DAYCOUNT
};

extern const MG_ArgConv			DayCountNameConvertor;
extern const MG_ArgConvReverse	DayCountNameRevertor;


enum ADJRULE_NAME
{
	K_FIXED,
	K_FOLLOWING_PAY,					// Next business day, same interest period
	K_FOLLOWING,						// Next business day, modify interest period
	K_MODIFIED_FOLLOWING,				// Next business day unless month change then previous business day, modifiy interest period
	K_MODIFIED_FOLLOWING_NOREDEMP,	// Next business day unless month change then previous business day, same interest period
	K_PREVIOUS_PAY,					// Previous business day, same interest period
	K_PREVIOUS,						// Previous business day, modify interest period
	K_MODIFIED_PREVIOUS,				// Previous business day unless month change then next business day, modifiy interest period
	K_MODIFIED_PREVIOUS_NOREDEMP,		// Previous business day unless month change then next business day, same interest period

	NB_ADJRULE
};

extern const MG_ArgConv			AdjustmentRuleNameConvertor;
extern const MG_ArgConvReverse	AdjustmentRuleNameRevertor;


/* enum for interpolation methods */
enum INTERPOL_METHOD {
	NONE_INTERPOL,			// no interpolation
	STEPUPLEFT_INTERPOL,	// step up left interpol method
	STEPUPRIGHT_INTERPOL,	// step up right interpol method
	LIN_INTERPOL,			// linear interpol method
	POLYNOM_INTERPOL,		// polynomial interpol method
	CUBICSPLINE_INTERPOL,	// cubic spline interpol method
	CONSTANT_INTERPOL,		// constant interpol method

	NB_INTERPOL_METH
};

extern const MG_ArgConv			InterpolMethodConvertor;
extern const MG_ArgConvReverse	InterpolMethodRevertor;


/* enum for random generator types */
/* Perf: TAUS, GFSR4, MT19937, RANLXS0, RANLXS1, MRG, RANLUX, RANLXD1, RANLXS2, CMRG, RANLUX389, RANLXD2 */
enum RAND_TYPE {
	BOROSH13,
	CMRG, COVEYOU,
	FISHMAN18, FISHMAN20, FISHMAN2X, 
	GFSR4,
	KNUTHRAN, KNUTHRAN2, KNUTHRAN2002,
	LECUYER21,
	MINSTD, MRG,
	MT19937, MT19937_1999, MT19937_1998,
	R250, RAN0, RAN1, RAN2, RAN3, RAND, RAND48,
	RANDOM128_BSD, RANDOM128_GLIBC2, RANDOM128_LIBC5, 
	RANDOM256_BSD, RANDOM256_GLIBC2, RANDOM256_LIBC5,
	RANDOM32_BSD, RANDOM32_GLIBC2, RANDOM32_LIBC5,
	RANDOM64_BSD, RANDOM64_GLIBC2, RANDOM64_LIBC5,
	RANDOM8_BSD, RANDOM8_GLIBC2, RANDOM8_LIBC5, 
	RANDOM_BSD, RANDOM_GLIBC2, RANDOM_LIBC5,
	RANDU, RANF,
	RANLUX, RANLUX389, RANLXD1, RANLXD2, RANLXS0, RANLXS1, RANLXS2, RANMAR, 
	SLATEC,
	TAUS, TAUS2, TAUS113, TRANSPUTER, TT800,
	UNI, UNI32, 
	VAX,
	WATERMAN14,
	ZUF,

	NB_RAND_TYPE
};

extern const MG_ArgConv			RandGenConvertor;
extern const MG_ArgConvReverse	RandGenRevertor;


/* enum for quasi random generator types */
enum QUASIRAND_TYPE
{
	NIEDERREITER2,
	SOBOL,
	HALTON,
	REVERSEHALTON,

	NB_QUASIRAND_TYPE
};

extern const MG_ArgConv			QuasiRandGenConvertor;
extern const MG_ArgConvReverse	QuasiRandGenRevertor;


/* enum for quasi random generator types */
enum MODELPARAM_TYPE
{
	VOLATILITY,
	MEANREVERSION,
	CORRELATION,
	VOLVOL,

	NB_MODELPARAM_TYPE
};

extern const MG_ArgConv			ModleParamGenConvertor;
extern const MG_ArgConvReverse	ModleParamGenRevertor;


MG_NAMESPACE_END
