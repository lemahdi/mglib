#include "mgsecurity/fra.h"
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
{}

void MG_Fra::Swap(MG_Fra& aRight)
{
	MG_IRSecurity::Swap(aRight);
	swap(myRcvPay, aRight.myRcvPay);
	myIRIndex.Swap(aRight.myIRIndex);
	swap(myFixedRate, aRight.myFixedRate);
	std::swap(myRawFwd, aRight.myRawFwd);
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
	
	myForwards.resize(1);
	myFlows.resize(1);
	myDfs.resize(1);

	const MG_Date& vFwdStDt = mySchedule.GetFwdRateStartDates().front();
	const MG_Date& vFwdEdDt = mySchedule.GetFwdRateEndDates().front();
	const MG_Date& vPayDt = mySchedule.GetPayDates().front();

	myForwards[0] = myRawFwd = aMdl.Libor(vFwdStDt, vFwdEdDt, myIRIndex.GetDayCount(), myIRIndex.GetPayCalendar());
	myDfs[0] = aMdl.DiscountFactor(vPayDt);

	double vDelta = mySchedule.GetIntTerms().front();
	double vPayoff = (myRawFwd - myFixedRate) * vDelta;
	vPayoff /= (1 + vDelta*myRawFwd);
	myFlows[0] = vPayoff;
}

double MG_Fra::Price(void) const
{
	return myDfs[0] * myFlows[0];
}
