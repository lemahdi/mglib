/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.17
 * Date					: 29 DEC 2010
 * Purpose				: MG_Model is a base class for creating models
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/market/closedformulas.h"
#include "mgmktdata/robot.h"


MG_NAMESPACE_BEGIN


/* IR Pricing Functions */


/* Base class for models */
class MG_Model : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	MG_Model(void);
	// self-generated copy-constructor is enough
	virtual ~MG_Model(void);

	ASSIGN_OPERATOR(MG_Model)
	//CLONE_METHOD(MG_Model)
	SWAP_DECL(MG_Model)

	MG_Model(const MG_Date& aAsOf);

public:
	/* State */
	inline const MG_Date& GetAsOf(void) const { return myAsOf; }

	/* Market Data */
	virtual void Register(MG_RobotPtr& aRbt) = 0;

	/* Engine */
	virtual double DiscountFactor(const MG_Date& aMaturity) const = 0;
	virtual double Libor(	const MG_Date		& aStDt
						,	const MG_Date		& aEdDt
						,	const DAYCOUNT_NAME	& aDayCount
						,	const CALENDAR_NAME	& aCal) const = 0;

protected:
	MG_Date myAsOf;
};

/* Discount Factor model class */
class MG_DfModel : public MG_Model
{
public:
	/* Constructors / Destructor */
	//COPY_CTOR_DECL(MG_DfModel)

	ASSIGN_OPERATOR(MG_DfModel)
	SWAP_DECL(MG_DfModel)
	CLONE_METHOD(MG_DfModel)

	virtual ~MG_DfModel(void);

	MG_DfModel(const MG_Date& aAsOf);

public:
	/* Market Data */
	void Register(MG_RobotPtr& aRbt);

	/* Engine */
	double DiscountFactor(const MG_Date& aMaturity) const;
	double Libor(	const MG_Date		& aStDt
				,	const MG_Date		& aEdDt
				,	const DAYCOUNT_NAME	& aDayCount
				,	const CALENDAR_NAME	& aCal) const;

private:
	MG_ZeroCurvePtr myZC;

};

/* Black & Scholes model class */
class MG_BSModel : public MG_Model
{
public:
	/* Constructors / Destructor */
	//COPY_CTOR_DECL(MG_BSModel)

	ASSIGN_OPERATOR(MG_BSModel)
	SWAP_DECL(MG_BSModel)
	CLONE_METHOD(MG_BSModel)

	virtual ~MG_BSModel(void);

	MG_BSModel(const MG_Date& aAsOf);

public:
	/* Market Data */
	void Register(MG_RobotPtr& aRbt);

	/* Engine */
	double DiscountFactor(const MG_Date& aMaturity) const;
	double Libor(	const MG_Date		& aStDt
				,	const MG_Date		& aEdDt
				,	const DAYCOUNT_NAME	& aDayCount
				,	const CALENDAR_NAME	& aCal) const;
	double OptionPrice	(	const MG_CF::OPTION_TYPE& aOptType
						,	const double			&aFwd
						,	const double			& aTenorStrike
						,	const double			& aMaturity) const;

private:
	MG_ZeroCurvePtr			myZC;
	MG_VolatilityCurvePtr	myAtmVol;

};

MG_NAMESPACE_END
