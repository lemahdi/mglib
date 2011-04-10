#include "model.h"


using namespace std;
using namespace MG;


/* Base Model class */
MG_Model::MG_Model() : myAsOf(MG_Date())
{}

MG_Model::MG_Model	(	const MG_Date &aAsOf)
					:	myAsOf(aAsOf)
{}


/* Black & Scholes Model class */
MG_BSModel::MG_BSModel() : MG_Model(), myVol(0)
{}

MG_BSModel::MG_BSModel	(	const MG_Date &aAsOf, const double &aVol)
						:	MG_Model(aAsOf)
						,	myVol(aVol)
{}

double MG_BSModel::CallPrice(const double &aSpot, const double &aStrike, const double &aMaturity)
{aSpot, aStrike, aMaturity;
	return 0;
}

double MG_BSModel::PutPrice(const double &aSpot, const double &aStrike, const double &aMaturity)
{aSpot, aStrike, aMaturity;
	return 0;
}

