#include "mggenpricer/genmod/blackscholes.h"
#include "mgmktdata/marketdata.h"
#include "mgnova/utils/utils.h"

#include <math.h>


using namespace std;
using namespace MG;


/*
 * Vasicek Model Class
 */
MG_BlackScholes::MG_BlackScholes(	const MG_BlackScholes& aRight)
								:	MG_IRPricingModel(aRight)
								,	myZC(aRight.myZC)
								,	myVol(aRight.myVol)
{}

void MG_BlackScholes::Swap(MG_BlackScholes& aRight)
{
	MG_PricingModel::Swap(aRight);
	myZC.Swap(aRight.myZC);
	myVol.Swap(aRight.myVol);
}

MG_BlackScholes::MG_BlackScholes(	const MG_ZeroCurvePtr& aZC
								,	const MG_VolatilityCurvePtr& aVol)
								:	MG_IRPricingModel()
								,	myZC(aZC)
								,	myVol(aVol)
{}

MG_BlackScholes::~MG_BlackScholes()
{}

vector<double> MG_BlackScholes::Libor	(	const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
										,	const double& aDelta, const double& aTenor, const double& aSpread
										,	const vector<double>& aStates)
{
	double vAsOfJul = myZC->AsOf().GetJulianDay();
	double vMat = (aRstDt.GetJulianDay()-vAsOfJul)/365.;
	size_t vNbStates = aStates.size();
	vector<double> vLibors(vNbStates);
	double vLibor = myZC->Libor((aStDt.GetJulianDay()-vAsOfJul)/365., (aEdDt.GetJulianDay()-vAsOfJul)/365., aDelta);
	double vVol = myVol->ComputeValue(aTenor, vMat);
	double vExpVar = exp(-0.5*vVol*vVol*vMat);
	double vSqrt = vVol*sqrt(vMat);
	for(size_t i=0; i<vNbStates; ++i)
		vLibors[i] = vLibor * vExpVar * exp(vSqrt*aStates[i]) + aSpread;
	return vLibors;
}

//==> One coupon pricing
vector<double> MG_BlackScholes::Caplet	(	const MG_Date& aMatDt
										,	const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
										,	const double& aDelta, const double& aTenor, const double& aSpread
										,	const double& aStrike, const vector<double>& aStates)
{
	vector<double> vLibors = Libor(aRstDt, aStDt, aEdDt, aPayDt, aDelta, aTenor, aSpread, aStates);
	size_t vNbStates = aStates.size();
	vector<double> vPayoffs(vNbStates);
	for(size_t i=0; i<vNbStates; ++i)
		vPayoffs[i] = std::max(vLibors[i]-aStrike, 0.);
	return vPayoffs;
}
vector<double> MG_BlackScholes::Floorlet(	const MG_Date& aMatDt
										,	const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
										,	const double& aDelta, const double& aTenor, const double& aSpread
										,	const double& aStrike, const vector<double>& aStates)
{
	vector<double> vLibors = Libor(aRstDt, aStDt, aEdDt, aPayDt, aDelta, aTenor, aSpread, aStates);
	size_t vNbStates = aStates.size();
	vector<double> vPayoffs(vNbStates);
	for(size_t i=0; i<vNbStates; ++i)
		vPayoffs[i] = std::max(aStrike-vLibors[i], 0.);
	return vPayoffs;
}
vector<double> MG_BlackScholes::DigitalUp	(	const MG_Date& aMatDt
											,	const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
											,	const double& aDelta, const double& aTenor, const double& aSpread
											,	const double& aStrike, const double& aAlpha
											,	const vector<double>& aStates)
{
	vector<double> vLibors = Libor(aRstDt, aStDt, aEdDt, aPayDt, aDelta, aTenor, aSpread, aStates);
	size_t vNbStates = aStates.size();
	vector<double> vPayoffs(vNbStates);
	for(size_t i=0; i<vNbStates; ++i)
	{
		int vCond = vLibors[i]>aStrike ? 1 : 0;
		vPayoffs[i] = aAlpha * vCond;
	}
	return vPayoffs;
}
vector<double> MG_BlackScholes::DigitalDown	(	const MG_Date& aMatDt
											,	const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
											,	const double& aDelta, const double& aTenor, const double& aSpread
											,	const double& aStrike, const double& aAlpha
											,	const vector<double>& aStates)
{
	vector<double> vLibors = Libor(aRstDt, aStDt, aEdDt, aPayDt, aDelta, aTenor, aSpread, aStates);
	size_t vNbStates = aStates.size();
	vector<double> vPayoffs(vNbStates);
	for(size_t i=0; i<vNbStates; ++i)
	{
		int vCond = vLibors[i]<aStrike ? 1 : 0;
		vPayoffs[i] = aAlpha * vCond;
	}
	return vPayoffs;
}

//==> Set of coupons pricing
vector<double> MG_BlackScholes::Cap(const MG_Date& aMatDt
								,	const MG_Schedule& aSched, const double& aTenor, const vector<double>& aSpread
								,	const double& aStrike, const vector<double>& aStates)
{
	return vector<double>();
}
vector<double> MG_BlackScholes::Floor(const MG_Date& aMatDt
								,	const MG_Schedule& aSched, const double& aTenor, const vector<double>& aSpread
								,	const double& aStrike, const vector<double>& aStates)
{
	return vector<double>();
}

//==> Other
vector<double> MG_BlackScholes::Swaption(const MG_Date& aMatDt, const MG_Schedule& aSched, const double& aTenor
								,	const double& aStrike, const vector<double>& aStates)
{
	return vector<double>();
}