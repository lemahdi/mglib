/*
 * Copyright			: 2011 by MG
 * File					: marketdata.h
 * Version				: 0.1.21
 * Last changed			: 25 MAR 2011
 * Purpose				: MG_MarketData is a base class for market datas
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "date.h"
#include "matrix.h"
#include "interpolator.h"


MG_NAMESPACE_BEGIN


/* Base class for market datas */
class MG_MarketData : public MG_Object
{
public:
	/* Constructors / Destructor */
	//MG_MarketData(void);
	MG_MarketData(const MG_MarketData& aRight);
	ASSIGN_OPERATOR(MG_MarketData)
	//CLONE_METHOD(MG_MarketData)
	SWAP_DECL(MG_MarketData)
	virtual ~MG_MarketData(void);

	MG_MarketData(const MG_Date& aAsOf, const INTERPOL_METHOD& aInterpolMethod = LIN_INTERPOL);

public:
	virtual double ComputeValue(const double& aX = 0, const double& aY = 0, const double& aZ = 0) = 0;

protected:
	MG_Date				myAsOf;
	MG_InterpolatorPtr	myInterpolator;
};


class MG_ZeroCurve : public MG_MarketData
{
	typedef std::vector<double> MG_ABSC;

public:
	/* Constructors / Destructor */
	MG_ZeroCurve(void);
	MG_ZeroCurve(const MG_ZeroCurve& aRight);
	ASSIGN_OPERATOR(MG_ZeroCurve)
	CLONE_METHOD(MG_ZeroCurve)
	SWAP_DECL(MG_ZeroCurve)
	virtual ~MG_ZeroCurve(void);

	MG_ZeroCurve(	const MG_Date			& aAsOf
				,	const MG_ABSC			& aMaturities
				,	const MG_Matrix			& aCurve
				,	const INTERPOL_METHOD	& aInterpolMethod = LIN_INTERPOL);

public:
	virtual double ComputeValue(const double& aMaturity = 0, const double& aY = 0, const double& aZ = 0);

private:
	MG_ABSC				myMaturities;
	MG_Matrix			myCurve;
};


class MG_IRVolatilityCurve : public MG_MarketData
{
	typedef std::vector<double> MG_ABSC;
	typedef std::vector<double> MG_COOR;

public:
	/* Constructors / Destructor */
	MG_IRVolatilityCurve(void);
	MG_IRVolatilityCurve(const MG_IRVolatilityCurve& aRight);
	ASSIGN_OPERATOR(MG_IRVolatilityCurve)
	CLONE_METHOD(MG_IRVolatilityCurve)
	SWAP_DECL(MG_IRVolatilityCurve)
	virtual ~MG_IRVolatilityCurve(void);

	MG_IRVolatilityCurve(	const MG_Date			& aAsOf
						,	const MG_ABSC			& aMaturities
						,	const MG_ABSC			& aTenors
						,	const MG_Matrix			& aCurve
						,	const INTERPOL_METHOD	& aInterpolMethod = LIN_INTERPOL);

public:
	virtual double ComputeValue(const double& aMaturity = 0, const double& aTenor = 0, const double& aZ = 0);

private:
	MG_ABSC				myMaturities;
	MG_COOR				myTenors;
	MG_Matrix			myCurve;
};



MG_NAMESPACE_END
