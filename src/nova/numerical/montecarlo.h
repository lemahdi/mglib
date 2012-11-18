/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.23
 * Date					: 11 DEC 2011
 * Purpose				: MG_MonteCarlo is a base class for dealing with multidimensional Monte Carlo integration
 * Author				: MM Akkouh
 * Notes				: The statistical estimate of the error on the result should be taken as a guide rather than a strict error bound
 */


#pragma once


#include "nova/glob/typedef.h"
#include "nova/glob/object.h"
#include "nova/patterns/countedptr.hpp"
#include "nova/wrapper/function.h"

#include "gsl/gsl_monte_plain.h"
#include "gsl/gsl_monte_miser.h"
#include "gsl/gsl_monte_vegas.h"


MG_NAMESPACE_BEGIN


#define MCDensityPtr CountedPtr<MG_MonteCarlo::MCDensity>
class MG_Random;


/* MonteCarlo Class */
class MG_MonteCarlo : public MG_XLObject
{
public:
	struct MCDensity
	{
		SWAP_DECL(MCDensity)

		void Load(const size_t& aDim);
		static double Aire(double* aX, size_t aDim, void* aParams);
		virtual double AireFunction(double* aX, size_t aDim, void* aParams) = 0;

		gsl_monte_function myFunc;
	};
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_MonteCarlo)

	ASSIGN_OPERATOR(MG_MonteCarlo)
	//CLONE_METHOD(MG_MonteCarlo)
	SWAP_DECL(MG_MonteCarlo)

	MG_MonteCarlo(const size_t& aDim);

	virtual ~MG_MonteCarlo(void);

	/* State */
	void Load(const MCDensityPtr& aDensity, const MG_RandomPtr& aRandGen);

	/* Engine */
	inline double EvaluateFunction(double* aX) { return GSL_MONTE_FN_EVAL(&myDensity->myFunc, aX); }
	virtual double Integrate(double aLow[], double aUp[], double& aErr, const size_t& aCalls = 100) = 0;

protected:
	size_t	myDim;

	MCDensityPtr myDensity;
	MG_RandomPtr myRandGen;

};


/* Plain MC */
class MG_PlainMC : public MG_MonteCarlo
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_PlainMC)

	ASSIGN_OPERATOR(MG_PlainMC)
	CLONE_METHOD(MG_PlainMC)
	SWAP_DECL(MG_PlainMC)

	MG_PlainMC(const size_t& aDim);

	virtual ~MG_PlainMC(void);

	/* Engine */
	double Integrate(double aLow[], double aUp[], double& aErr, const size_t& aCalls = 100);

private:
	gsl_monte_plain_state* myWorkSpace;

};


/* Miser MC */
/* See documentation for more functions */
class MG_MiserMC : public MG_MonteCarlo
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_MiserMC)

	ASSIGN_OPERATOR(MG_MiserMC)
	CLONE_METHOD(MG_MiserMC)
	SWAP_DECL(MG_MiserMC)

	MG_MiserMC(const size_t& aDim);

	virtual ~MG_MiserMC(void);

	/* Engine */
	double Integrate(double aLow[], double aUp[], double& aErr, const size_t& aCalls = 100);

private:
	gsl_monte_miser_state* myWorkSpace;

};


/* Vegas MC */
class MG_VegasMC : public MG_MonteCarlo
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_VegasMC)

	ASSIGN_OPERATOR(MG_VegasMC)
	CLONE_METHOD(MG_VegasMC)
	SWAP_DECL(MG_VegasMC)

	MG_VegasMC(const size_t& aDim);

	virtual ~MG_VegasMC(void);

	/* Engine */
	double Integrate(double aLow[], double aUp[], double& aErr, const size_t& aCalls = 100);

private:
	gsl_monte_vegas_state* myWorkSpace;

};

MG_NAMESPACE_END

