#include "mgsecurity/swapleg.h"
#include "mgnova/utils/utils.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* IR Swap Leg class */
MG_SwapLeg::MG_SwapLeg	(	const MG_SwapLeg& aRight)
						:	MG_IRSecurity(aRight)
						,	myRcvPay(aRight.myRcvPay)
{}

void MG_SwapLeg::Swap(MG_SwapLeg& aRight)
{
	MG_IRSecurity::Swap(aRight);
	swap(myRcvPay, aRight.myRcvPay);
}

MG_SwapLeg::MG_SwapLeg	(	const MG_Date		& aStDt
						,	const MG_Date		& aEdDt
						,	const RCVPAY_NAME	& aRcvPay
						,	const MG_IRIndex	& aIRIndex)
						:	MG_IRSecurity()
						,	myRcvPay(aRcvPay)
{
	myXLName = MG_SWAPLEG_XL_NAME;

	FREQUENCY_NAME vFreq = MG_utils::GetFrequencyFromIndex(aIRIndex.GetIndexName());
	mySchedule = MG_Schedule(aStDt, aEdDt, aIRIndex, vFreq);
}

MG_SwapLeg::MG_SwapLeg	(	const MG_GenericDate& aStDt
						,	const MG_GenericDate& aEdDt
						,	const RCVPAY_NAME	& aRcvPay
						,	const MG_IRIndex	& aIRIndex)
						:	MG_IRSecurity()
						,	myRcvPay(aRcvPay)
{
	myXLName = MG_SWAPLEG_XL_NAME;

	FREQUENCY_NAME vFreq = MG_utils::GetFrequencyFromIndex(aIRIndex.GetIndexName());
	mySchedule = MG_Schedule(aStDt, aEdDt, aIRIndex, vFreq);
}

MG_SwapLeg::MG_SwapLeg	(	const MG_Schedule& aSched
						,	const RCVPAY_NAME& aRcvPay)
						:	MG_IRSecurity(aSched)
						,	myRcvPay(aRcvPay)
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
	for(size_t i=0; i<vNbFlows; ++i)
	{
		myRawFwd[i] = aMdl.Libor(vFwdStDts[i], vFwdEdDts[i]
								, mySchedule.GetIRIndex().GetDayCount(), mySchedule.GetIRIndex().GetPayCalendar());
		myDfs[i] = aMdl.DiscountFactor(vPayDts[i]);
	}
}

double MG_SwapLeg::Price(void) const
{
	return myRawFwd.SumProduct(myDfs);
}
