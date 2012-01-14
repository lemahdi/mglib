#include "mgsecurity/swapleg.h"
#include "mgnova/utils/utils.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


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

	FREQUENCY_NAME vFreq = MG_utils::GetFrequencyFromIndex(aIRIndex.GetIndexName());
	mySchedule = MG_Schedule(aStDt, aEdDt, aIRIndex, vFreq);
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

	myRawFwd.Clear();
	myRawFwd.Resize(vNbFlows);
	myDfs.Clear();
	myDfs.Resize(vNbFlows);

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
		myRawFwd = MG_Vector(vNbFlows);
		for(size_t i=0; i<vNbFlows; ++i)
			myDfs[i] = aMdl.DiscountFactor(vPayDts[i]);
	}
}

double MG_SwapLeg::Price(void) const
{
	MG_Vector vFwd = myRawFwd + mySpreadOrRate;
	return vFwd.SumProduct(myDfs);
}
