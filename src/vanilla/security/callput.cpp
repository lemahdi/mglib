#include "vanilla/security/callput.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Call / Put option class */
MG_CallPut::MG_CallPut(	const MG_CallPut& aRight)
						:	MG_VanillaOption(aRight)
						,	myMatDt		(aRight.myMatDt)
						,	myCallPut	(aRight.myCallPut)
{}

void MG_CallPut::Swap(MG_CallPut& aRight)
{
	MG_VanillaOption::Swap(aRight);
	myMatDt.Swap(aRight.myMatDt);
	std::swap(myCallPut, aRight.myCallPut);
}

MG_CallPut::MG_CallPut	(	const MG_GenericDate& aMatDt
						,	const MG_Security	& aUnderlying
						,	const CALLPUT_NAME	& aCallPut
						,	const double		& aStrike)
						:	MG_VanillaOption(vector<MG_Date>(), aUnderlying, aStrike)
						,	myMatDt		(aMatDt)
						,	myCallPut	(aCallPut)
{
	myXLName = MG_CALLPUT_XL_NAME;
}

MG_CallPut::MG_CallPut	(	const vector<MG_Date>	& aMatDts
						,	const MG_Security		& aUnderlying
						,	const CALLPUT_NAME		& aCallPut
						,	const double			& aStrike)
						:	MG_VanillaOption(aMatDts, aUnderlying, aStrike)
						,	myCallPut(aCallPut)
{
	myXLName = MG_CALLPUT_XL_NAME;
}

MG_CallPut::~MG_CallPut()
{}

void MG_CallPut::PrePricing(const MG_Model& aMdl)
{
	myUnderlying->PrePricing(aMdl);
	const MG_Date& vAsOf = aMdl.GetAsOf();
	double vAsOfJul = vAsOf.GetJulianDay();

	if (myMatDt.GetTimes() != 0)
	{
		myMatDt.Rebuild(vAsOf);
		myMatDts.resize(1);
		myMatDts[0] = myMatDt;
	}

	size_t vNbFlows = myMatDts.size();

	myForwards.resize(vNbFlows);
	myFlows.resize(vNbFlows);
	myDfs.resize(vNbFlows);

	const vector<double>& vUndFwds = myUnderlying->Forwards();
	double vMat(0.);
	for(size_t i=0; i<vNbFlows; ++i)
	{
		myForwards[i] = vUndFwds[i];
		const MG_Date& vMatDt = myMatDts[i];
		vMat = (vMatDt.GetJulianDay() - vAsOfJul) / 365.;
		myFlows[i] = aMdl.OptionPrice(myCallPut==K_CALL ? MG_CF::CALL : MG_CF::PUT, myForwards[i], 0., vMat);
		myDfs[i] = aMdl.DiscountFactor(vMatDt);
	}
}

double MG_CallPut::Price(void) const
{
	size_t vNbFlows = myForwards.size();

	double vPrice(0.);
	for(size_t i=0; i<vNbFlows; ++i)
		vPrice += myDfs[i] * myFlows[i];

	return vPrice;
}
