#include "mgmodel/model.h"
#include <math.h>


using namespace std;
using namespace MG;


/* Base Model class */
MG_Model::MG_Model() : MG_XLObject(), myAsOf(MG_Date())
{}

void MG_Model::Swap(MG_Model& aRight)
{
	MG_XLObject::Swap(aRight);
	myAsOf.Swap(aRight.myAsOf);
}

MG_Model::MG_Model	(	const MG_Date &aAsOf)
					:	MG_XLObject(), myAsOf(aAsOf)
{}

MG_Model::~MG_Model()
{}


/* Discount Factor Model class */
MG_DfModel::MG_DfModel	(	const MG_Date &aAsOf)
						:	MG_Model(aAsOf)
						,	myZC(NULL)
{
	myXLName = MG_DFMODEL_XL_NAME;
}

MG_DfModel::~MG_DfModel()
{}

void MG_DfModel::Swap(MG_DfModel& aRight)
{
	MG_Model::Swap(aRight);
	myZC.Swap(aRight.myZC);
}

void MG_DfModel::Register(MG_RobotPtr& aRbt)
{
	myZC = aRbt->GetMktData("ZERO", "EUR", "EURIB");
}

double MG_DfModel::DiscountFactor(const MG_Date& aMaturity) const
{
	double vMat = (aMaturity - myAsOf) / 365.;
	return myZC->DiscountFactor(vMat);
}

double MG_DfModel::Libor(	const MG_Date		& aStDt
						,	const MG_Date		& aEdDt
						,	const DAYCOUNT_NAME	& aDayCount
						,	const CALENDAR_NAME	& aCal) const
{
	double vDelta = aEdDt.BetweenDays(aStDt, aDayCount, true, aCal);
	double vMatS = (aStDt-myAsOf) / 365.;
	double vMatE = (aEdDt-myAsOf) / 365.;
	return myZC->Libor(vMatS, vMatE, vDelta);
}


/* Black & Scholes Model class */
MG_BSModel::MG_BSModel	(	const MG_Date &aAsOf)
						:	MG_Model(aAsOf)
						,	myZC	(NULL)
						,	myAtmVol(NULL)
{
	myXLName = MG_BSMODEL_XL_NAME;
}

MG_BSModel::~MG_BSModel()
{}

void MG_BSModel::Swap(MG_BSModel& aRight)
{
	MG_Model::Swap(aRight);
	myZC.Swap(aRight.myZC);
	myAtmVol.Swap(aRight.myAtmVol);
}

void MG_BSModel::Register(MG_RobotPtr& aRbt)
{
	myZC = aRbt->GetMktData("ZERO", "EUR", "EURIB");
	myAtmVol = aRbt->GetMktData("IRVOL", "EUR", "EURIB");
}

double MG_BSModel::DiscountFactor(const MG_Date& aMaturity) const
{
	double vMat = (aMaturity-myAsOf) / 365.;
	return myZC->DiscountFactor(vMat);
}

double MG_BSModel::Libor(	const MG_Date		& aStDt
						,	const MG_Date		& aEdDt
						,	const DAYCOUNT_NAME	& aDayCount
						,	const CALENDAR_NAME	& aCal) const
{
	double vDelta = aEdDt.BetweenDays(aStDt, aDayCount, true, aCal);
	double vMatS = (aStDt-myAsOf) / 365.;
	double vMatE = (aEdDt-myAsOf) / 365.;
	return myZC->Libor(vMatS, vMatE, vDelta);
}

double MG_BSModel::CallPrice(const double &aFwd, const double &aTenorStrike, const double &aMaturity)
{
	double vDf = myZC->DiscountFactor(aMaturity);
	double vVol = myAtmVol->ComputeValue(aTenorStrike, aMaturity);
	return MG_CF::CallPrice(aFwd, aTenorStrike, aMaturity, vDf, vVol);
}

double MG_BSModel::PutPrice(const double &aFwd, const double &aTenorStrike, const double &aMaturity)
{	
	double DF = myZC->DiscountFactor(aMaturity);
	double vVol = myAtmVol->ComputeValue(aTenorStrike, aMaturity);
	return MG_CF::PutPrice(aFwd, aTenorStrike, aMaturity, DF, vVol);
}

