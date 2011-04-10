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


#include "date.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_Model
{
public:
	/* Constructors / Destructor */
	MG_Model(void);
	MG_Model(const MG_Date& aAsOf);

private:
	MG_Date myAsOf;
};

/* Black & Scholes model class */
class MG_BSModel : public MG_Model
{
public:
	MG_BSModel(void);
	MG_BSModel(const MG_Date& aAsOf, const double& aVol);

	/* Functions */
	double CallPrice(const double& aSpot, const double& aStrike, const double& aMaturity);
	double PutPrice(const double& aSpot, const double& aStrike, const double& aMaturity);

private:
	double myVol;
};


MG_NAMESPACE_END
