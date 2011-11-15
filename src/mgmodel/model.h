/*
 * Copyright			: 2010 by MG
 * File					: model.h
 * Version				: 0.1.17
 * Last changed			: 29 DEC 2010
 * Purpose				: MG_Model is a base class for creating models
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnumerical/closedformulas.h"
#include "mgmktdata/robot.h"


MG_NAMESPACE_BEGIN


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
	/* Market Data */
	virtual void Register(MG_RobotPtr& aRbt) = 0;

private:
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

	/* Functions */
	double DiscountFactor(const double& aMaturity);

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

	/* Functions */
	double CallPrice(const double& aFwd, const double& aTenorStrike, const double& aMaturity);
	double PutPrice(const double& aFwd, const double& aTenorStrike, const double& aMaturity);

private:
	MG_ZeroCurvePtr			myZC;
	MG_VolatilityCurvePtr	myAtmVol;

};

MG_NAMESPACE_END
