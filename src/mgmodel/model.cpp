#include "mgmodel/model.h"
#include <math.h>


using namespace std;
using namespace MG;


/* Base Model class */
MG_Model::MG_Model() : MG_XLObject(), myAsOf(MG_Date())
{}

void MG_Model::Swap(MG_Model& aRight)
{
	myAsOf.Swap(aRight.myAsOf);
}

MG_Model::MG_Model	(	const MG_Date &aAsOf)
					:	MG_XLObject(), myAsOf(aAsOf)
{}

MG_Model::~MG_Model()
{}


/* Black & Scholes Model class */
MG_BSModel::MG_BSModel() : MG_Model(), myVol(0)
{
	myXLName = MG_BSMODEL_XL_NAME;
}

MG_BSModel::MG_BSModel	(	const MG_Date &aAsOf, const double &aVol)
						:	MG_Model(aAsOf)
						,	myVol(aVol)
{
	myXLName = MG_BSMODEL_XL_NAME;
}

MG_BSModel::~MG_BSModel()
{}

void MG_BSModel::Swap(MG_BSModel& aRight)
{
	MG_Model::Swap(aRight);
	swap(myVol, aRight.myVol);
}

double MG_BSModel::CallPrice(const double &aFwd, const double &aStrike, const double &aMaturity)
{
	double DF = exp(-0.03*aMaturity);
	return VanillaPrice(aFwd, aStrike, aMaturity, DF, myVol, _call);
}

double MG_BSModel::PutPrice(const double &aFwd, const double &aStrike, const double &aMaturity)
{	
	double DF = exp(-0.03*aMaturity);
	return VanillaPrice(aFwd, aStrike, aMaturity, DF, myVol, _put);
}

