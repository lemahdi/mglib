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
