#include "mggenpricer/genmod/irpricingmodel.h"
#include "mgmktdata/marketdata.h"


using namespace std;
using namespace MG;


/*
 * IR Pricing Model Base Class
 */
MG_IRPricingModel::MG_IRPricingModel(	const MG_IRPricingModel& aRight)
									:	MG_PricingModel(aRight)
									,	myZC(aRight.myZC)
{}

void MG_IRPricingModel::Swap(MG_IRPricingModel& aRight)
{
	MG_PricingModel::Swap(aRight);
	myZC.Swap(aRight.myZC);
}

MG_IRPricingModel::MG_IRPricingModel(const MG_ZeroCurvePtr& aZC) : MG_PricingModel(), myZC(aZC)
{}

MG_IRPricingModel::~MG_IRPricingModel()
{}

const MG_Date& MG_IRPricingModel::AsOf(void) const { return myZC->AsOf(); }
const MG_ZeroCurvePtr& MG_IRPricingModel::ZcCurve(void) const { return myZC; }

double MG_IRPricingModel::Libor(const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& /*aPayDt*/, const double& aDelta)
{
	double vAsOfJul = AsOf().GetJulianDay();
	double vMatSt = (aStDt.GetJulianDay()-vAsOfJul) / 365.;
	double vMatEd = (aEdDt.GetJulianDay()-vAsOfJul) / 365.;
	return myZC->Libor(vMatSt, vMatEd, aDelta);
}
