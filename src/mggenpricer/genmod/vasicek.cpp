#include "mggenpricer/genmod/vasicek.h"


using namespace std;
using namespace MG;


/*
 * Vasicek Model Class
 */
MG_Vasicek::MG_Vasicek(const MG_Vasicek& aRight) : MG_IRPricingModel(aRight)
{}

void MG_Vasicek::Swap(MG_Vasicek& aRight)
{
	MG_PricingModel::Swap(aRight);
}

MG_Vasicek::MG_Vasicek() : MG_IRPricingModel()
{}

MG_Vasicek::~MG_Vasicek()
{}

vector<double> MG_Vasicek::Libor(const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
					, const double& aDelta, const double& aTenor, const double& aSpread
					, const vector<double>& aStates)
{
	return vector<double>();
}

//==> One coupon pricing
vector<double> MG_Vasicek::Caplet(const MG_Date& aMatDt
					, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
					, const double& aDelta, const double& aTenor, const double& aSpread
					, const double& aStrike, const vector<double>& aStates)
{
	return vector<double>();
}
vector<double> MG_Vasicek::Floorlet(const MG_Date& aMatDt
					, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
					, const double& aDelta, const double& aTenor, const double& aSpread
					, const double& aStrike, const vector<double>& aStates)
{
	return vector<double>();
}
vector<double> MG_Vasicek::DigitalUp(const MG_Date& aMatDt
					, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
					, const double& aDelta, const double& aTenor, const double& aSpread
					, const double& aStrike, const double& aAlpha
					, const vector<double>& aStates)
{
	return vector<double>();
}
vector<double> MG_Vasicek::DigitalDown(const MG_Date& aMatDt
					, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
					, const double& aDelta, const double& aTenor, const double& aSpread
					, const double& aStrike, const double& aAlpha
					, const vector<double>& aStates)
{
	return vector<double>();
}

//==> Set of coupons pricing
vector<double> MG_Vasicek::Cap(const MG_Date& aMatDt
					, const MG_Schedule& aSched, const double& aTenor, const vector<double>& aSpread
					, const double& aStrike
					, const vector<double>& aStates)
{
	return vector<double>();
}
vector<double> MG_Vasicek::Floor(const MG_Date& aMatDt
					, const MG_Schedule& aSched, const double& aTenor, const vector<double>& aSpread
					, const double& aStrike
					, const vector<double>& aStates)
{
	return vector<double>();
}

//==> Other
vector<double> MG_Vasicek::Swaption(const MG_Date& aMatDt, const MG_Schedule& aSched, const double& aTenor
					, const double& aStrike
					, const vector<double>& aStates)
{
	return vector<double>();
}