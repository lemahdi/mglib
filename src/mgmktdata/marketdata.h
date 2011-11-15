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

#include "gsl/gsl_interp.h"
#include "gsl/gsl_spline.h"


MG_NAMESPACE_BEGIN


/* Base class for market datas */
class MG_MarketData : public MG_XLObject
{
protected:
	typedef MG_Matrix MG_Line;
	typedef MG_Matrix MG_Curve;
	typedef MG_Vector MG_ABSC;
	typedef MG_Vector MG_ORD;

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_MarketData)

	ASSIGN_OPERATOR(MG_MarketData)
	SWAP_DECL(MG_MarketData)

	MG_MarketData	(	const MG_Date		& aAsOf
					,	const std::string	& aType
					,	const std::string	& aCcy
					,	const std::string	& aUnderIndex
					,	const int			& aInterpolTypes = 0);

	virtual ~MG_MarketData(void);

public:
	virtual double ComputeValue(const double& aX = 0, const double& aY = 0, const double& aZ = 0) = 0;

	inline std::string GetType		(void) const { return myType; }
	inline std::string GetCurrency	(void) const { return myCurrency; }
	inline std::string GetUnderIndex(void) const { return myUnderIndex; }

protected:
	MG_Date		myAsOf;
	const int	myInterpolTypes;

	std::string	myType;
	std::string	myCurrency;
	std::string	myUnderIndex;
};


/* Empty Market Data - Useful for Robot */
class MG_EmptyMD : public MG_MarketData
{
public:
	CLONE_METHOD(MG_EmptyMD)

	MG_EmptyMD(const std::string& aType, const std::string& aCcy, const std::string& aUnderIndex);

	double ComputeValue(const double& aX = 0, const double& aY = 0, const double& aZ = 0);
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

	MG_ZeroCurve(	const MG_Date		& aAsOf
				,	const MG_ABSC		& aMaturities
				,	const MG_Line		& aCurve
				,	const std::string	& aCcy
				,	const std::string	& aUnderIndex
				,	const int			& aInterpolTypes);

	virtual ~MG_ZeroCurve(void);

public:
	virtual double ComputeValue(const double& aMaturity = 0, const double& aY = 0, const double& aZ = 0);

private:
	MG_ABSC				myMaturities;
	MG_Line				myCurve;

	gsl_spline*			myInterpolator;
};


/* Volatility Curve */
class MG_VolatilityCurve : public MG_MarketData
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_VolatilityCurve)

	ASSIGN_OPERATOR(MG_VolatilityCurve)
	SWAP_DECL(MG_VolatilityCurve)

	MG_VolatilityCurve	(	const MG_Date		& aAsOf
						,	const std::string	& aType
						,	const std::string	& aCcy
						,	const std::string	& aUnderIndex
						,	const int			& aInterpolTypes = 0);

	virtual ~MG_VolatilityCurve(void);

protected:
	std::vector<gsl_spline*>	my1stInterps;
	gsl_interp*					my2ndInterp;
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

	MG_IRVolatilityCurve(	const MG_Date		& aAsOf
						,	const MG_ABSC		& aMaturities
						,	const MG_ORD		& aTenors
						,	const MG_Matrix		& aCurve
						,	const std::string	& aCcy
						,	const std::string	& aUnderIndex
						,	const int			& aInterpolTypes);

public:
	virtual double ComputeValue(const double& aTenor = 0, const double& aMaturity = 0, const double& aZ = 0);

private:
	MG_ABSC				myMaturities;
	MG_ORD				myTenors;
	MG_Matrix			myCurve;
	MG_Matrix			myTransCurve;

};

/* Dividends - Dividends Table*/
class MG_DividendsTable : public MG_MarketData
{
	typedef MG_Vector MG_ABSC;
	typedef MG_Vector MG_Line;

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
					,	const std::string		& aCcy
					,	const std::string		& aUnderIndex
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

	MG_EQVolatilityCurve(	const MG_Date		& aAsOf
						,	const MG_ABSC		& aStrikes
						,	const MG_ORD		& aMaturities
						,	const MG_Matrix		& aCurve
						,	const std::string	& aCcy
						,	const std::string	& aUnderIndex
						,	const int			& aInterpolTypes = interpoltypeLinear);

public:
	virtual double ComputeValue(const double& aStrike = 0, const double& aMaturity = 0, const double& aZ = 0);

private:
	MG_ABSC				myStrikes;
	MG_ORD				myMaturities;
	MG_Matrix			myCurve;
	MG_Matrix			myTransCurve;

};


MG_NAMESPACE_END
