#include "vanilla/security/swapleg.h"
#include "vanilla/model/model.h"
#include "nova/utils/utils.h"


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
}

MG_SwapLeg::MG_SwapLeg	(	const MG_GenericDate	& aStDt
						,	const MG_GenericDate	& aEdDt
						,	const RCVPAY_NAME		& aRcvPay
						,	const MG_IRIndex		& aIRIndex
						,	const double			& aSpdOrRate
						,	const NX_NAME			& aNX
						,	const MG_TermStructure	& aNotional)
						:	MG_IRSecurity(aNX, aNotional)
						,	myRcvPay		(aRcvPay)
						,	myIRIndex		(aIRIndex)
						,	mySpreadOrRate	(aSpdOrRate)
{
	myXLName = MG_SWAPLEG_XL_NAME;
	GenerateSchedule(aStDt, aEdDt, aIRIndex);
}

MG_SwapLeg::MG_SwapLeg	(	const MG_Schedule		& aSched
						,	const RCVPAY_NAME		& aRcvPay
						,	const double			& aSpdOrRate
						,	const NX_NAME			& aNX
						,	const MG_TermStructure	& aNotional)
						:	MG_IRSecurity(aSched, aNX, aNotional)
						,	myRcvPay		(aRcvPay)
						,	myIRIndex		(aSched.GetIRIndex())
						,	mySpreadOrRate	(aSpdOrRate)
{
	myXLName = MG_SWAPLEG_XL_NAME;
}

MG_SwapLeg::~MG_SwapLeg()
{}

double MG_SwapLeg::Level(const MG_Model& aMdl) const
{
	MG_SwapLeg vSwapLeg(*this);
	vSwapLeg.PrePricing(aMdl);

	MG_Schedule& vSched = vSwapLeg.mySchedule;
	vector<double> vDeltaDfNot = vSched.GetIntTerms();
	const vector<double>& vDfs = vSwapLeg.Dfs();
	VectorMult(vDeltaDfNot, vDfs);
	const vector<double>& vNotionals = vSwapLeg.Notionals();
	VectorMult(vDeltaDfNot, vNotionals);

	double vLvl = VectorSum(vDeltaDfNot);
	return vLvl;
}

void MG_SwapLeg::PrePricing(const MG_Model& aMdl)
{
	mySchedule.InterpretDates(aMdl.GetAsOf());
	size_t vNbFlows = mySchedule.NbFlows();

	myRawFwd.resize(vNbFlows);
	myDfs.resize(vNbFlows);
	myNotionals.resize(vNbFlows);

	const vector<MG_Date>& vPayDts = mySchedule.GetPayDates();
	
	for(size_t i=0; i<vNbFlows; ++i)
	{
		const MG_Date& vPayDt = vPayDts[i];
		myDfs[i] = aMdl.DiscountFactor(vPayDt);
		myNotionals[i] = myNotional.CptValue(vPayDt);
	}
	if (!IsFixed())
	{
		const vector<MG_Date>& vFwdStDts = mySchedule.GetFwdRateStartDates();
		const vector<MG_Date>& vFwdEdDts = mySchedule.GetFwdRateEndDates();
		for(size_t i=0; i<vNbFlows; ++i)
			myRawFwd[i] = aMdl.Libor(vFwdStDts[i], vFwdEdDts[i]
									, myIRIndex.GetDayCount(), myIRIndex.GetPayCalendar());
	}
}

double MG_SwapLeg::Price(void) const
{
	vector<double> vDeltaFwdNot(myRawFwd);
	const vector<double>& vDelta(mySchedule.GetIntTerms());

	VectorPlus(vDeltaFwdNot, mySpreadOrRate);
	VectorMult(vDeltaFwdNot, vDelta);
	VectorMult(vDeltaFwdNot, myDfs);
	double vPrice = VectorSumProduct(vDeltaFwdNot, myNotionals);
	return myRcvPay * vPrice;
}

