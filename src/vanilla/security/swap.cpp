#include "vanilla/security/swap.h"
#include "vanilla/model/model.h"
#include "nova/glob/exception.h"
#include "nova/utils/utils.h"


using namespace std;
using namespace MG;
using namespace MG_utils::Vector;


/* IR Swap Leg class */
MG_Swap::MG_Swap(	const MG_Swap& aRight)
				:	MG_IRSecurity(aRight)
				,	myRcvPay(aRight.myRcvPay)
				,	myLeg1	(aRight.myLeg1)
				,	myLeg2	(aRight.myLeg2)
{}

void MG_Swap::Swap(MG_Swap& aRight)
{
	MG_IRSecurity::Swap(aRight);
	swap(myRcvPay, aRight.myRcvPay);
	myLeg1.Swap(aRight.myLeg1);
	myLeg2.Swap(aRight.myLeg2);
}

MG_Swap::MG_Swap(	const MG_SwapLeg	& aLeg1
				,	const MG_SwapLeg	& aLeg2
				,	const RCVPAY_NAME	& aRcvPay)
				:	MG_IRSecurity()
				,	myRcvPay(aRcvPay)
				,	myLeg1	(aLeg1)
				,	myLeg2	(aLeg2)
{
	myXLName = MG_SWAP_XL_NAME;
}

MG_Swap::~MG_Swap()
{}

const MG_SwapLeg& MG_Swap::FixedLeg() const
{
	bool vLeg1IsFix = myLeg1.IsFixed();
	bool vLeg2IsFix = myLeg2.IsFixed();
	if (vLeg2IsFix == vLeg2IsFix)
		MG_THROW("Swap: both legs are fixed/float.");

	if (vLeg1IsFix)
		return myLeg1;
	return myLeg2;
}

void MG_Swap::PrePricing(const MG_Model& aMdl)
{
	myLeg1.PrePricing(aMdl);
	myLeg2.PrePricing(aMdl);
}

double MG_Swap::Price(void) const
{
	double vLeg1 = myLeg1.Price();
	double vLeg2 = myLeg2.Price();

	return vLeg1 + vLeg2;
}

double MG_Swap::SwapRate(const MG_Model& aMdl) const
{
	MG_Swap vSwap(*this);
	const MG_SwapLeg& vFixedLeg = vSwap.FixedLeg();
	const MG_SwapLeg& vFloatLeg = vSwap.FloatLeg();
	vSwap.PrePricing(aMdl);
	double vLvl = vFixedLeg.Level(aMdl);
	double vSwapRate = vFloatLeg.Price() / vLvl;
	return vSwapRate;
}

double MG_Swap::ImpliedSpread(const MG_Model& aMdl, const size_t& aLegId, const double& aPrice) const
{
	assert(aLegId==1 || aLegId==2 && ": Leg Id should be 1 or 2");
	if (aLegId==1 && !myLeg1.IsFloat())
		MG_THROW("Swap: Cannot compute an implied rate upon a fixed leg.");
	if (aLegId==2 && !myLeg2.IsFloat())
		MG_THROW("Swap: Cannot compute an implied rate upon a fixed leg.");

	MG_Swap vSwap(*this);
	MG_SwapLeg& vLeg1 = vSwap.myLeg1;
	vLeg1.Spread(0.);
	MG_SwapLeg& vLeg2 = vSwap.myLeg2;
	vLeg2.Spread(0.);
	vSwap.PrePricing(aMdl);

	double vPrice0 = vSwap.Price();
	double vLvl(0.);
	if (aLegId == 1)
		vLvl = vLeg1.Level(aMdl);
	else
		vLvl = vLeg2.Level(aMdl);
	double vSpread = (aPrice - vPrice0) / vLvl;

	return vSpread;
}

double MG_Swap::ImpliedRate(const MG_Model& aMdl, const size_t& aLegId, const double& aPrice) const
{
	assert(aLegId==1 || aLegId==2 && ": Leg Id should be 1 or 2");
	if (aLegId==1 && !myLeg1.IsFixed())
		MG_THROW("Swap: Cannot compute an implied rate upon a float leg.");
	if (aLegId==2 && !myLeg2.IsFixed())
		MG_THROW("Swap: Cannot compute an implied rate upon a float leg.");

	MG_Swap vSwap(*this);
	vSwap.PrePricing(aMdl);
	const MG_SwapLeg& vLeg1 = vSwap.Leg1();
	const MG_SwapLeg& vLeg2 = vSwap.Leg2();

	double vLvl(0.), vOtherPrice(0.);
	if (aLegId == 1)
	{
		vLvl = vLeg1.Level(aMdl);
		vOtherPrice = vLeg2.Price();
	}
	else
	{
		vLvl = vLeg2.Level(aMdl);
		vOtherPrice = vLeg1.Price();
	}
	double vRate = (aPrice - vOtherPrice) / vLvl;

	return vRate;
}
