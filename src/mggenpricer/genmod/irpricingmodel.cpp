#include "mggenpricer/genmod/irpricingmodel.h"


using namespace std;
using namespace MG;


/*
 * IR Pricing Model Base Class
 */
MG_IRPricingModel::MG_IRPricingModel(const MG_IRPricingModel& aRight) : MG_PricingModel(aRight)
{}

void MG_IRPricingModel::Swap(MG_IRPricingModel& aRight)
{
	MG_PricingModel::Swap(aRight);
}

MG_IRPricingModel::MG_IRPricingModel() : MG_PricingModel()
{}

MG_IRPricingModel::~MG_IRPricingModel()
{}
