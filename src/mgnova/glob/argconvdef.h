/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.17
 * Date					: 25 DEC 2010
 * Purpose				: Specializations of MG_ArgConv  and MG_ArgConvReverse
 * Author				: MM Akkouh
 * Notes				: 3 letters months name
 *						  3 letters currencies name
 */


#pragma once


#include "mgnova/glob/argconv.h"


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


enum INDEX_NAME
{
	K_FIXED,

	K_OIS,
	K_EONIA,
	K_SONIA,

	K_LIMIT_MM,	// money market limit

	K_LIBOR1M,
	K_LIBOR3M,
	K_LIBOR6M,
	K_LIBOR12M,

	K_EUBOR1M,
	K_EUBOR3M,
	K_EUBOR6M,
	K_EUBOR12M,

	K_LIMIT_FWD,	// libor limit

	K_CMS1,
	K_CMS2,
	K_CMS3,
	K_CMS4,
	K_CMS5,
	K_CMS6,
	K_CMS7,
	K_CMS8,
	K_CMS9,
	K_CMS10,
	K_CMS11,
	K_CMS12,
	K_CMS13,
	K_CMS14,
	K_CMS15,
	K_CMS16,
	K_CMS17,
	K_CMS18,
	K_CMS19,
	K_CMS20,
	K_CMS21,
	K_CMS22,
	K_CMS23,
	K_CMS24,
	K_CMS25,
	K_CMS26,
	K_CMS27,
	K_CMS28,
	K_CMS29,
	K_CMS30,
	K_CMS31,
	K_CMS32,
	K_CMS33,
	K_CMS34,
	K_CMS35,
	K_CMS36,
	K_CMS37,
	K_CMS38,
	K_CMS39,
	K_CMS40,
	K_CMS41,
	K_CMS42,
	K_CMS43,
	K_CMS44,
	K_CMS45,
	K_CMS46,
	K_CMS47,
	K_CMS48,
	K_CMS49,
	K_CMS50,

	NB_INDEX
};
#define INDEX_NAME_DEF K_EUBOR6M
#define INDEX_NAME_DEF_STR "EUBOR6M"

extern const MG_ArgConv			IndexNameConvertor;
extern const MG_ArgConvReverse	IndexNameRevertor;


enum CURRENCY_NAME
{
	K_USD,
	K_EUR,
	K_JPY,
	K_GBP,

	NB_CURRENCIES
};
#define CURRENCY_NAME_DEF K_EUR
#define CURRENCY_NAME_DEF_STR "EUR"

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
#define CALENDAR_NAME_DEF_STR "EUR"

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
	K_YEARLY = 1,

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


enum STUBRULE_NAME
{
	K_SHORT_START,
	K_LONG_START,
	K_SHORT_END,
	K_LONG_END,

	NB_STUBRULE
};

extern const MG_ArgConv			StubRuleNameConvertor;
extern const MG_ArgConvReverse	StubRuelNameRevertor;


enum ADJ_NAME
{
	K_ADJUSTED,
	K_UNADJUSTED,

	NB_ADJ
};

extern const MG_ArgConv			AdjustmentNameConvertor;
extern const MG_ArgConvReverse	AdjustmentNameRevertor;


enum ADJRULE_NAME
{
	K_FIXED_RULE,
	K_FOLLOWING_PAY,				// Next business day, same interest period
	K_FOLLOWING,					// Next business day, modify interest period
	K_MODIFIED_FOLLOWING,			// Next business day unless month change then previous business day, modifiy interest period
	K_MODIFIED_FOLLOWING_NOREDEMP,	// Next business day unless month change then previous business day, same interest period
	K_PREVIOUS_PAY,					// Previous business day, same interest period
	K_PREVIOUS,						// Previous business day, modify interest period
	K_MODIFIED_PREVIOUS,			// Previous business day unless month change then next business day, modifiy interest period
	K_MODIFIED_PREVIOUS_NOREDEMP,	// Previous business day unless month change then next business day, same interest period

	NB_ADJRULE
};

extern const MG_ArgConv			AdjustmentRuleNameConvertor;
extern const MG_ArgConvReverse	AdjustmentRuleNameRevertor;


enum TIMING_NAME
{
	K_ADVANCE,
	K_ARREARS,

	NB_TIMING
};

extern const MG_ArgConv			TimingNameConvertor;
extern const MG_ArgConvReverse	TimingNameRevertor;


enum RCVPAY_NAME
{
	K_PAY = -1,
	K_RCV = 1,

	NB_RCVPAY = 2
};

extern const MG_ArgConv			RcvPayNameConvertor;
extern const MG_ArgConvReverse	RcvPayNameRevertor;


/* enum for interpolation methods */
enum INTERPOL_METHOD {
	NONE_INTERPOL			= interpoltypeNone,			// no interpolation
	CONSTANT_INTERPOL		= interpoltypeConstant,		// constant interpol method
	STEPUPLEFT_INTERPOL		= interpoltypeStepUpLeft,	// step up left interpol method
	STEPUPRIGHT_INTERPOL	= interpoltypeStepUpRight,	// step up right interpol method
	LINEAR_INTERPOL			= interpoltypeLinear,		// linear interpol method
	CONTINUOUS_INTERPOL		= interpoltypeContinuous,	// continuous interpol method
	POLYNOM_INTERPOL		= interpoltypePolynomial,	// polynomial interpol method
	CUBICSPLINE_INTERPOL	= interpoltypeCubicSpline,	// cubic spline interpol method
	AKIMA_INTERPOL			= interpoltypeAkima,		// non rounded corner algorithm of Wodicka

	NB_INTERPOL_METH		= 9
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
