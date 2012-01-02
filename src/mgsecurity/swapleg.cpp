#include "mgsecurity/swapleg.h"
#include "mgnova/utils/utils.h"


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
	FREQUENCY_NAME vFreq = MG_utils::GetFrequencyFromIndex(aIRIndex.GetIndexName());
	mySchedule = MG_Schedule(aStDt, aEdDt, aIRIndex, vFreq);
}

MG_SwapLeg::MG_SwapLeg	(	const MG_Schedule& aSched
						,	const RCVPAY_NAME& aRcvPay)
						:	MG_IRSecurity(aSched)
						,	myRcvPay(aRcvPay)
{}

MG_SwapLeg::~MG_SwapLeg()
{}

