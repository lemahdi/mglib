#include "vanilla/security/digital.h"
#include "vanilla/model/model.h"
#include "nova/utils/utils.h"


using namespace std;
using namespace MG;
using namespace MG_utils::Vector;


/* Forward Rate Agreement class */
MG_Digital::MG_Digital	(	const MG_Digital& aRight)
						:	MG_VanillaOption(aRight)
						,	myMatDt		(aRight.myMatDt)
						,	myCallPut	(aRight.myCallPut)
{}

void MG_Digital::Swap(MG_Digital& aRight)
{
	MG_VanillaOption::Swap(aRight);
	myMatDt.Swap(aRight.myMatDt);
	std::swap(myCallPut, aRight.myCallPut);
}

MG_Digital::MG_Digital	(	const MG_GenericDate& aMatDt
						,	const MG_SecurityPtr& aUnderlying
						,	const CALLPUT_NAME	& aCallPut
						,	const double		& aStrike)
						:	MG_VanillaOption(vector<MG_Date>(), aUnderlying, aStrike)
						,	myMatDt		(aMatDt)
						,	myCallPut	(aCallPut)
{
	myXLName = MG_DIGITAL_XL_NAME;
}

MG_Digital::MG_Digital	(	const vector<MG_Date>	& aMatDts
						,	const MG_SecurityPtr	& aUnderlying
						,	const CALLPUT_NAME		& aCallPut
						,	const double			& aStrike)
						:	MG_VanillaOption(aMatDts, aUnderlying, aStrike)
						,	myCallPut(aCallPut)
{
	myXLName = MG_DIGITAL_XL_NAME;
}

MG_Digital::~MG_Digital()
{}

void MG_Digital::PrePricing(const MG_Model& aMdl)
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
		myFlows[i] = aMdl.OptionPrice(myCallPut==K_CALL ? MG_CF::DIGITAL_CALL : MG_CF::DIGITAL_PUT, myForwards[i], 0., vMat);
		myDfs[i] = aMdl.DiscountFactor(vMatDt);
	}
}

double MG_Digital::Price(void) const
{
	vector<double> vFlowsPV = myFlows;
	double vPrice = VectorSumProduct(vFlowsPV, myDfs);
	return vPrice;
}
