#include "mgsecurity/fra.h"
#include "mgnova/utils/utils.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Forward Rate Agreement class */
MG_Fra::MG_Fra	(	const MG_Fra& aRight)
				:	MG_IRSecurity(aRight)
				,	myRcvPay	(aRight.myRcvPay)
				,	myIRIndex	(aRight.myIRIndex)
				,	myFixedRate	(aRight.myFixedRate)
				,	myRawFwd	(0.)
				,	myDf		(0.)
{}

void MG_Fra::Swap(MG_Fra& aRight)
{
	MG_IRSecurity::Swap(aRight);
	swap(myRcvPay, aRight.myRcvPay);
	myIRIndex.Swap(aRight.myIRIndex);
	swap(myFixedRate, aRight.myFixedRate);
}

MG_Fra::MG_Fra	(	const MG_GenericDate& aStDt
				,	const MG_GenericDate& aEdDt
				,	const RCVPAY_NAME	& aRcvPay
				,	const MG_IRIndex	& aIRIndex
				,	const double		& aFixedRate)
				:	MG_IRSecurity()
				,	myRcvPay		(aRcvPay)
				,	myIRIndex		(aIRIndex)
				,	myFixedRate		(aFixedRate)
{
	myXLName = MG_FRA_XL_NAME;
	//!!! here the schedule should pay in advance (PayDate = StartDate)
	GenerateSchedule(aStDt, aEdDt, aIRIndex);
}

MG_Fra::~MG_Fra()
{}

void MG_Fra::PrePricing(const MG_Model& aMdl)
{
	mySchedule.InterpretDates(aMdl.GetAsOf());

	const MG_Date& vFwdStDt = mySchedule.GetFwdRateStartDates().front();
	const MG_Date& vFwdEdDt = mySchedule.GetFwdRateEndDates().front();
	const MG_Date& vPayDt = mySchedule.GetPayDates().front();

	myRawFwd = aMdl.Libor(vFwdStDt, vFwdEdDt, myIRIndex.GetDayCount(), myIRIndex.GetPayCalendar());
	myDf = aMdl.DiscountFactor(vPayDt);
}

double MG_Fra::Price(void) const
{
	const double& vDelta = mySchedule.GetIntTerms().front();
	double vPayoff = (myRawFwd - myFixedRate) * vDelta;
	vPayoff /= (1 + vDelta*myRawFwd);
	return myDf * vPayoff;
}
