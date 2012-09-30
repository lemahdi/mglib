/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.20
 * Date					: 01 JAN 2011
 * Purpose				: 
 * Author				: MM Akkouh
 * Notes				: 
 *
 * Updates				:  22 AUG 2011 by MM Akkouh - replacing old algorithms by GSL generators
 */


#pragma once


#include <vector>

#include "mgnova/glob/object.h"
#include "mgnova/glob/argconvdef.h"
#include "mgnova/wrapper/vector.h"

#include "gsl/gsl_rng.h"
#include "gsl/gsl_qrng.h"


MG_NAMESPACE_BEGIN


class MG_AbstractRandom : public MG_XLObject
{

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_AbstractRandom)

	ASSIGN_OPERATOR(MG_AbstractRandom)
	SWAP_DECL(MG_AbstractRandom)

	MG_AbstractRandom(const std::string& aName = "");

	virtual ~MG_AbstractRandom(void) {}

	/* Accessors */
	inline std::string GetName(void) const { return myName; }

	/* Support */
	virtual void Reset		(void)				= 0;
	virtual void ToString	(FILE* aFile) const	= 0;

	/* Engine */
	virtual double	DrawOne		(void)					= 0;
	virtual double	DrawUniform	(void)					= 0;
	virtual void	Draw		(MG_Vector& aValues)	= 0;

protected:
	std::string myName;
};


class MG_Random : public MG_AbstractRandom
{
	//Perf: TAUS, GFSR4, MT19937, RANLXS0, RANLXS1, MRG, RANLUX, RANLXD1, RANLXS2, CMRG, RANLUX389, RANLXD2
	static const gsl_rng_type* From_MGType_To_GSLType(const RAND_TYPE& aType);

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Random)

	ASSIGN_OPERATOR(MG_Random)
	CLONE_METHOD(MG_Random)
	SWAP_DECL(MG_Random)

	MG_Random(const RAND_TYPE& aType);

	virtual ~MG_Random(void);

	/* Accessors */
	inline size_t			GetMin		(void) const { return gsl_rng_min(myGen); }
	inline size_t			GetMax		(void) const { return gsl_rng_max(myGen); }
	inline gsl_rng*			GetGenerator(void) const { return myGen; }

	inline void				SetSeed		(const size_t& aSeed) { myInitialSeed = aSeed; gsl_rng_set(myGen, aSeed); }

	/* Support */
	void Reset(void);
	void ToString(FILE* aFile) const;

	/* Engine */
	double	DrawOne			(void)					{ return gsl_rng_get(myGen); }
	size_t	DrawOne			(const size_t& aMax)	{ return gsl_rng_uniform_int(myGen, aMax); }
	double	DrawUniform		(void)					{ return gsl_rng_uniform(myGen); }
	double	DrawUniformPos	(void)					{ return gsl_rng_uniform_pos(myGen); }
	size_t	DrawUniformInt	(const size_t& aMax)	{ return gsl_rng_uniform_int(myGen, aMax); }
	void	Draw			(MG_Vector& aValues);

private:
	size_t		myInitialSeed;
	gsl_rng*	myGen;

};


class MG_QuasiRandom : public MG_AbstractRandom
{
	static const gsl_qrng_type* From_MGType_To_GSLType(const QUASIRAND_TYPE& aType);

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_QuasiRandom)

	ASSIGN_OPERATOR(MG_QuasiRandom)
	CLONE_METHOD(MG_QuasiRandom)
	SWAP_DECL(MG_QuasiRandom)

	MG_QuasiRandom(const QUASIRAND_TYPE& aType, const size_t& aDim);

	virtual ~MG_QuasiRandom(void);

	/* Accessors */
	inline size_t			GetSize		(void) const { return gsl_qrng_size(myGen); }
	inline const gsl_qrng*	GetGenerator(void) const { return myGen; }

	/* Support */
	void Reset(void);
	void ToString(FILE* aFile) const;

	/* Engine */
	double	DrawOne		(void) { return DrawUniform(); }
	double	DrawUniform	(void);
	void	Draw		(MG_Vector& aValues);

private:
	gsl_qrng* myGen;
	MG_Vector myValues;

};


MG_NAMESPACE_END
