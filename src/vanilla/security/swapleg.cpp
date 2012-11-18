#include "vanilla/security/swapleg.h"
#include "vanilla/model/model.h"
#include "mgnova/utils/utils.h"


using namespace std;
using namespace MG;
using namespace MG_utils::Vector;


/* IR Swap Leg class */
MG_SwapLeg::MG_SwapLeg	(	const MG_SwapLeg& aRight)
						:	MG_IRSecurity(aRight)
						,	myRcvPay		(aRight.myRcvPay)
						,	myIRIndex		(aRight.myIRIndex)
						,	mySpreadOrRate	(aRight.mySpreadOrRate)
{}

void MG_SwapLeg::Swap(MG_SwapLeg& aRight)
{
	MG_IRSecurity::Swap(aRight);
	swap(myRcvPay, aRight.myRcvPay);
	myIRIndex.Swap(aRight.myIRIndex);
	swap(mySpreadOrRate, aRight.mySpreadOrRate);
	myRawFwd.swap(aRight.myRawFwd);
	myDfs.swap(aRight.myDfs);
}

MG_SwapLeg::MG_SwapLeg	(	const MG_GenericDate& aStDt
						,	const MG_GenericDate& aEdDt
						,	const RCVPAY_NAME	& aRcvPay
						,	const MG_IRIndex	& aIRIndex
						,	const double		& aSpdOrRate)
						:	MG_IRSecurity()
						,	myRcvPay		(aRcvPay)
						,	myIRIndex		(aIRIndex)
						,	mySpreadOrRate	(aSpdOrRate)
{
	myXLName = MG_SWAPLEG_XL_NAME;
	GenerateSchedule(aStDt, aEdDt, aIRIndex);
}

MG_SwapLeg::MG_SwapLeg	(	const MG_Schedule	& aSched
						,	const RCVPAY_NAME	& aRcvPay
						,	const double		& aSpdOrRate)
						:	MG_IRSecurity(aSched)
						,	myRcvPay		(aRcvPay)
						,	myIRIndex		(aSched.GetIRIndex())
						,	mySpreadOrRate	(aSpdOrRate)
{
	myXLName = MG_SWAPLEG_XL_NAME;
}

MG_SwapLeg::~MG_SwapLeg()
{}

void MG_SwapLeg::PrePricing(const MG_Model& aMdl)
{
	mySchedule.InterpretDates(aMdl.GetAsOf());
	size_t vNbFlows = mySchedule.GetResetDates().size();

	myRawFwd.clear();
	myRawFwd.resize(vNbFlows);
	myDfs.clear();
	myDfs.resize(vNbFlows);

	const vector<MG_Date>& vFwdStDts = mySchedule.GetFwdRateStartDates();
	const vector<MG_Date>& vFwdEdDts = mySchedule.GetFwdRateEndDates();
	const vector<MG_Date>& vPayDts = mySchedule.GetPayDates();
	if (myIRIndex.GetIndexName() != K_FIXED)
	{
		for(size_t i=0; i<vNbFlows; ++i)
		{
			myRawFwd[i] = aMdl.Libor(vFwdStDts[i], vFwdEdDts[i]
									, myIRIndex.GetDayCount(), myIRIndex.GetPayCalendar());
			myDfs[i] = aMdl.DiscountFactor(vPayDts[i]);
		}
	}
	else
	{
		myRawFwd.resize(vNbFlows);
		for(size_t i=0; i<vNbFlows; ++i)
			myDfs[i] = aMdl.DiscountFactor(vPayDts[i]);
	}
}

double MG_SwapLeg::Price(void) const
{
	vector<double> vDeltaFwd(myRawFwd);
	const vector<double>& vDelta(mySchedule.GetIntTerms());

	VectorPlus(vDeltaFwd, mySpreadOrRate);
	VectorMult(vDeltaFwd, vDelta);
	return VectorSumProduct(vDeltaFwd, myDfs);
}

vector<double> MG_SwapLeg::Forward() const
{
	return myRawFwd;
}
