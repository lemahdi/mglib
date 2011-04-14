#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Base Model class */
MG_Model::MG_Model() : MG_Object(), myAsOf(MG_Date())
{}

MG_Model::MG_Model(const MG_Model &aRight) : MG_Object(), myAsOf(aRight.myAsOf)
{}

void MG_Model::Swap(MG_Model& aRight)
{
	myAsOf.Swap(aRight.myAsOf);
}

MG_Model::MG_Model	(	const MG_Date &aAsOf)
					:	MG_Object(), myAsOf(aAsOf)
{}

MG_Model::~MG_Model()
{}


/* Black & Scholes Model class */
MG_BSModel::MG_BSModel() : MG_Model(), myVol(0)
{}

MG_BSModel::MG_BSModel	(	const MG_Date &aAsOf, const double &aVol)
						:	MG_Model(aAsOf)
						,	myVol(aVol)
{}

MG_BSModel::~MG_BSModel()
{}

void MG_BSModel::Swap(MG_BSModel& aRight)
{
	MG_Model::Swap(aRight);
	swap(myVol, aRight.myVol);
}

double MG_BSModel::CallPrice(const double &aFwd, const double &aStrike, const double &aMaturity)
{aFwd, aStrike, aMaturity;
	return 0;
}

double MG_BSModel::PutPrice(const double &aFwd, const double &aStrike, const double &aMaturity)
{aFwd, aStrike, aMaturity;
	return 0;
}

