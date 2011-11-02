#include "mggenpricer/genmod/pricingmodel.h"


using namespace std;
using namespace MG;


/*
 * Pricing Model Base Class
 */
MG_PricingModel::MG_PricingModel(const MG_PricingModel& aRight) : MG_XLObject(aRight)
{}

void MG_PricingModel::Swap(MG_PricingModel& aRight)
{}

MG_PricingModel::MG_PricingModel()
{}

MG_PricingModel::~MG_PricingModel()
{}
