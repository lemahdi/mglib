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


#include "mgnova/date.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_Model : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	MG_Model(void);
	MG_Model(const MG_Model& aRight);
	ASSIGN_OPERATOR(MG_Model)
	CLONE_METHOD(MG_Model)
	SWAP_DECL(MG_Model)
	virtual ~MG_Model(void);

	MG_Model(const MG_Date& aAsOf);

private:
	MG_Date myAsOf;
};

/* Black & Scholes model class */
class MG_BSModel : public MG_Model
{
public:
	MG_BSModel(void);
	// self-generated copy-constructor is enough
	ASSIGN_OPERATOR(MG_BSModel)
	CLONE_METHOD(MG_BSModel)
	SWAP_DECL(MG_BSModel)
	virtual ~MG_BSModel(void);

	MG_BSModel(const MG_Date& aAsOf, const double& aVol);

public:
	/* Functions */
	double CallPrice(const double& aFwd, const double& aStrike, const double& aMaturity);
	double PutPrice(const double& aFwd, const double& aStrike, const double& aMaturity);

private:
	double myVol;
};

class MG_Call : public MG_XLObject
{
public:
	MG_Call(const double& aK, const double& aMat, const double& aFwd)
		: myStrike(aK), myMaturity(aMat), myFwd(aFwd) { myXLName = "CALL"; }
	virtual MG_Object* Clone(void) { return NULL; }

public:
	double myStrike;
	double myMaturity;
	double myFwd;
};

MG_NAMESPACE_END
