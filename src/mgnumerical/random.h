/*
 * Copyright			: 2010 by MG
 * File					: random.h
 * Version				: 0.1.20
 * Last changed			: 01 JAN 2011
 * Purpose				: 
 * Author				: MM Akkouh
 * Notes				: 
 *
 * Updates				:  22 AUG 2011 by MM Akkouh - replacing old algorithms by GSL generators
 */


#pragma once


#include <vector>

#include "mgnova/object.h"
#include "gsl/gsl_rng.h"


MG_NAMESPACE_BEGIN


class MG_Random
{
	//Perf: TAUS, GFSR4, MT19937, RANLXS0, RANLXS1, MRG, RANLUX, RANLXD1, RANLXS2, CMRG, RANLUX389, RANLXD2

public:
	enum RAND_TYPE
	{
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
		ZUF
	};

	static const gsl_rng_type* From_MGType_To_GSLType(const RAND_TYPE& aType);

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Random)

	ASSIGN_OPERATOR(MG_Random)
	SWAP_DECL(MG_Random)

	MG_Random(const RAND_TYPE& aType);

	virtual ~MG_Random(void);

	/* Accessors */
	inline std::string		GetName		(void) const { std::string vName(gsl_rng_name(myGen)); return vName; }
	inline size_t			GetMin		(void) const { return gsl_rng_min(myGen); }
	inline size_t			GetMax		(void) const { return gsl_rng_max(myGen); }
	inline const gsl_rng*	GetGenerator(void) const { return myGen; }

	inline void				SetSeed		(const size_t& aSeed) { gsl_rng_set(myGen, aSeed); }

	/* Support */
	void ToString(FILE* aFile) const;

	/* Engine */
	inline size_t Draw			(void)					{ return gsl_rng_get(myGen); }
	inline size_t Draw			(const size_t& aMax)	{ return gsl_rng_uniform_int(myGen, aMax); }
	inline double DrawUniform	(void)					{ return gsl_rng_uniform(myGen); }
	inline double DrawUniformPos(void)					{ return gsl_rng_uniform_pos(myGen); }
	inline size_t DrawUniformInt(const size_t& aMax)	{ return gsl_rng_uniform_int(myGen, aMax); }

private:
	gsl_rng* myGen;

};


MG_NAMESPACE_END
