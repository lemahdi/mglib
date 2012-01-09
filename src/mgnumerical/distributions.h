/*
 * Copyright			: 2011 by MG
 * File					: normal.h
 * Version				: 0.1.21
 * Last changed			: 05 MAR 2011
 * Purpose				: simple class to deal with normal distribution fucntion
 * Author				: MM Akkouh
 * Notes				: 
 *
 * Updates				: 13 MAR 2011 by G El Boury - more consistent cumulative function
 *						: 22 AUG 2011 by MM Akkouh - replacing old functions by GSL functions
 */


#pragma once


#include <iostream>
#include <vector>

#include "mgnova/object.h"
#include "mgnova/patterns/countedptr.hpp"
#include "mgnumerical/random.h"

#include "gsl/gsl_randist.h"
#include "gsl/gsl_cdf.h"


MG_NAMESPACE_BEGIN


/* Random Distributions Abstract Class */
class MG_RandDist : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_RandDist)

	ASSIGN_OPERATOR(MG_RandDist)
	SWAP_DECL(MG_RandDist)

	MG_RandDist(const MG_RandomPtr& aRandGen);

	virtual ~MG_RandDist(void) {}

public:
	/* Engine */
	virtual double Density		(const double& aX)							= 0;
	virtual double DensityTail	(const double& aX, const double& aLimit)	= 0;
	virtual double Cdf			(const double& aX)							= 0;
	virtual double InvCdf		(const double& aP)							= 0;
	virtual double Draw			(void)										= 0;
	virtual double DrawTail		(const double& aLimit)						= 0;

protected:
	MG_RandomPtr myRandGen;

};


/* Normal Distribution Class */
class MG_NormalDist : public MG_RandDist
{
public:
	enum NOR_METH { NONE, ZIGGURAT, RATIO };
	typedef double (*MG_GaussRandFunc) (const gsl_rng* aGSLRndFunc, double aSigma);

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_NormalDist)

	ASSIGN_OPERATOR(MG_NormalDist)
	CLONE_METHOD(MG_NormalDist)
	SWAP_DECL(MG_NormalDist)

	MG_NormalDist(const MG_RandomPtr& aRandGen, const NOR_METH& aMeth = NONE, const double& aSigma = 1);

	virtual ~MG_NormalDist(void);

public:
	/* Engine */
	double Density		(const double& aX);
	double DensityTail	(const double& aX, const double& aLimit);
	double Cdf			(const double& aX);
	double InvCdf		(const double& aP);
	double Draw			(void);
	double DrawTail		(const double& aLimit);

	static double DensityFunc	(const double& aX);
	static double CdfFunc		(const double& aX);
	static double InvCdfFunc	(const double& aP);

private:
	NOR_METH			myMeth;
	double				mySigma;
	MG_GaussRandFunc	myFunc;

};


/* Uniform Distribution Class */
class MG_UniformDist : public MG_RandDist
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_UniformDist)

	ASSIGN_OPERATOR(MG_UniformDist)
	CLONE_METHOD(MG_UniformDist)
	SWAP_DECL(MG_UniformDist)

	MG_UniformDist(const MG_RandomPtr& aRandGen, const double& aA = 0., const double& aB = 1.);

	virtual ~MG_UniformDist(void) {}

public:
	/* Engine */
	double Density		(const double& aX);
	double DensityTail	(const double& aX, const double& aLimit) { return Density(aX); }
	double Cdf			(const double& aX);
	double InvCdf		(const double& aP);
	double Draw			(void);
	double DrawTail		(const double& aLimit) { return Draw(); }

private:
	double myA;
	double myB;

};


/* Lognormal Distribution Class */
class MG_LogNormalDist : public MG_RandDist
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_LogNormalDist)

	ASSIGN_OPERATOR(MG_LogNormalDist)
	CLONE_METHOD(MG_LogNormalDist)
	SWAP_DECL(MG_LogNormalDist)

	MG_LogNormalDist(const MG_RandomPtr& aRandGen, const double& aZeta, const double& aSigma);

	virtual ~MG_LogNormalDist(void) {}

public:
	/* Engine */
	double Density		(const double& aX);
	double DensityTail	(const double& aX, const double& aLimit) { return Density(aX); }
	double Cdf			(const double& aX);
	double InvCdf		(const double& aP);
	double Draw			(void);
	double DrawTail		(const double& aLimit) { return Draw(); }

private:
	double myZeta;
	double mySigma;

};


MG_NAMESPACE_END
