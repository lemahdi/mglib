#include "vanilla/security/swaption.h"
#include "vanilla/security/swapleg.h"
#include "vanilla/model/model.h"


using namespace std;
using namespace MG;


/* Forward Rate Agreement class */
MG_Swaption::MG_Swaption(	const MG_Swaption& aRight)
						:	MG_VanillaOption(aRight)
						,	myMatDt		(aRight.myMatDt)
						,	myCallPut	(aRight.myCallPut)
{}

void MG_Swaption::Swap(MG_Swaption& aRight)
{
	MG_VanillaOption::Swap(aRight);
	myMatDt.Swap(aRight.myMatDt);
	std::swap(myCallPut, aRight.myCallPut);
}

MG_Swaption::MG_Swaption(	const MG_GenericDate& aMatDt
						,	const MG_SwapPtr	& aUnderlying
						,	const CALLPUT_NAME	& aCallPut
						,	const double		& aStrike)
						:	MG_VanillaOption(vector<MG_Date>(), aUnderlying, aStrike)
						,	myMatDt		(aMatDt)
						,	myCallPut	(aCallPut)
{
	myXLName = MG_SWAPTION_XL_NAME;
}

MG_Swaption::~MG_Swaption()
{}

void MG_Swaption::PrePricing(const MG_Model& aMdl)
{
	myUnderlying->PrePricing(aMdl);
	const MG_Date& vAsOf = aMdl.GetAsOf();

	myMatDt.Rebuild(vAsOf);
	myMatDts.resize(1);
	myMatDts[0] = myMatDt;

	myForwards.resize(1);
	myFlows.resize(1);
	myDfs.resize(1);

	myForwards[0] = Swap()->SwapRate(aMdl);
	double vMat = (myMatDt - vAsOf) / 365.;
	myFlows[0] = aMdl.OptionPrice(myCallPut==K_CALL ? MG_CF::CALL : MG_CF::PUT, myForwards[0], 0., vMat);
	myDfs[0] = aMdl.DiscountFactor(myMatDts[0]);
}

double MG_Swaption::Price(void) const
{
	double vPrice = myFlows[0] * myDfs[0];
	return vPrice;
}
