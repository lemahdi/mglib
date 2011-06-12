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


#include "mgnova/date.h"
#include "mgmktdata/interpolator.h"
#include "xlw/MJmatrices.h"


MG_NAMESPACE_BEGIN


/* Base class for market datas */
class MG_MarketData : public MG_XLObject
{
protected:
	typedef xlw::MJMatrix		MG_Line;
	typedef xlw::MJMatrix		MG_Curve;
	typedef xlw::MJMatrix		MG_Matrix;
	typedef std::vector<double>	MG_ABSC;
	typedef std::vector<double>	MG_ORD;

	typedef double (*MG_InterpolLineFunc)	(const MG_Line&, const size_t&, const INTERPOL_DIM&, const MG_ABSC&, const double&);
	typedef double (*MG_InterpolMatrixFunc)	(const MG_Curve&, const MG_ABSC&, const MG_ORD&, const double&, const double&, const int&);

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_MarketData)

	ASSIGN_OPERATOR(MG_MarketData)
	SWAP_DECL(MG_MarketData)

	MG_MarketData(const MG_Date& aAsOf);

public:
	virtual double ComputeValue(const double& aX = 0, const double& aY = 0, const double& aZ = 0) = 0;

protected:
	MG_Date		myAsOf;
};


/* Zero Curve - Discount Factors Curve */
class MG_ZeroCurve : public MG_MarketData
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_ZeroCurve)

	ASSIGN_OPERATOR(MG_ZeroCurve)
	CLONE_METHOD(MG_ZeroCurve)
	SWAP_DECL(MG_ZeroCurve)

	MG_ZeroCurve(	const MG_Date	& aAsOf
				,	const MG_ABSC	& aMaturities
				,	const MG_Line	& aCurve
				,	const int		& aInterpolTypes);

public:
	virtual double ComputeValue(const double& aMaturity = 0, const double& aY = 0, const double& aZ = 0);

private:
	MG_ABSC				myMaturities;
	MG_Line				myCurve;

	MG_InterpolLineFunc	myInterpolFunc;
};


/* Volatility Curve */
class MG_VolatilityCurve : public MG_MarketData
{
public:
	/* Constructors / Destructor */
	MG_VolatilityCurve	(const MG_Date& aAsOf);

};

/* IR Volatility Curve */
class MG_IRVolatilityCurve : public MG_VolatilityCurve
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_IRVolatilityCurve)

	ASSIGN_OPERATOR(MG_IRVolatilityCurve)
	CLONE_METHOD(MG_IRVolatilityCurve)
	SWAP_DECL(MG_IRVolatilityCurve)

	MG_IRVolatilityCurve(	const MG_Date	& aAsOf
						,	const MG_ABSC	& aMaturities
						,	const MG_ORD	& aTenors
						,	const MG_Matrix	& aCurve
						,	const int		& aInterpolTypes);

public:
	virtual double ComputeValue(const double& aTenor = 0, const double& aMaturity = 0, const double& aZ = 0);

private:
	MG_ABSC				myMaturities;
	MG_ORD				myTenors;
	MG_Matrix			myCurve;

	MG_InterpolLineFunc	myMaturityInterpolFunc;
	MG_InterpolLineFunc	myTenorInterpolFunc;
};

/* Dividends - Dividends Table*/
class MG_DividendsTable : public MG_MarketData
{
	typedef std::vector<MG_Date>	MG_ABSC;
	typedef std::vector<double>		MG_Line;

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_DividendsTable)

	ASSIGN_OPERATOR(MG_DividendsTable)
	CLONE_METHOD(MG_DividendsTable)
	SWAP_DECL(MG_DividendsTable)

	MG_DividendsTable(	const MG_Date			& aAsOf
					,	const MG_ABSC			& aExDivDates
					,	const MG_ABSC			& aPaymentDates
					,	const MG_Line			& aCurve
					,	const MG_ZeroCurvePtr	& aZeroCurve);

public:
	virtual double ComputeValue(const double& aT1 = 0, const double& aT2 = 0, const double& aZ = 0);

private:
	MG_ABSC				myExDivDates;
	MG_ABSC				myPaymentDates;
	MG_Line				myCurve;
	MG_ZeroCurvePtr		myZeroCurve;
};

/* Equity Volatility Curve */
class MG_EQVolatilityCurve : public MG_VolatilityCurve
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_EQVolatilityCurve)

	ASSIGN_OPERATOR(MG_EQVolatilityCurve)
	CLONE_METHOD(MG_EQVolatilityCurve)
	SWAP_DECL(MG_EQVolatilityCurve)

	MG_EQVolatilityCurve(	const MG_Date	& aAsOf
						,	const MG_ABSC	& aStrikes
						,	const MG_ORD	& aMaturities
						,	const MG_Matrix	& aCurve
						,	const int		& aInterpolTypes = interpoltypeLinear);

public:
	virtual double ComputeValue(const double& aStrike = 0, const double& aMaturity = 0, const double& aZ = 0);

private:
	MG_ABSC				myStrikes;
	MG_ORD				myMaturities;
	MG_Matrix			myCurve;

	MG_InterpolLineFunc	myStrikeInterpolFunc;
	MG_InterpolLineFunc	myMaturityInterpolFunc;
};



MG_NAMESPACE_END
